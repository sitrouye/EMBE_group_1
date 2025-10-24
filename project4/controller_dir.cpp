#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gpiod.h>

// --- helpers ---
static inline int wstr(const char* p, const char* s){
  int fd = open(p, O_WRONLY);
  if(fd < 0){ perror(p); return -1; }
  if(write(fd, s, strlen(s)) < 0){ perror(p); close(fd); return -1; }
  close(fd); return 0;
}
static inline int wull(const char* p, unsigned long long v){
  char b[32]; snprintf(b, sizeof(b), "%llu", v); return wstr(p, b);
}
static inline double now_s(){
  struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char** argv){
  if(argc < 2){ fprintf(stderr, "Usage: %s <ref_rps (signed)>\n", argv[0]); return 1; }
  double ref_rps = atof(argv[1]);      // sign controls direction

  // ----- encoder open + reset -----
  int w = open("/dev/encoder0", O_WRONLY);
  if(w < 0){ perror("/dev/encoder0 (write)"); return 1; }
  write(w, "reset", 5); close(w);
  int enc = open("/dev/encoder0", O_RDONLY);
  if(enc < 0){ perror("/dev/encoder0 (read)"); return 1; }

  // ----- GPIO13 as DIRECTION (gpiod) -----
  struct gpiod_chip* chip = gpiod_chip_open_by_name("gpiochip0");
  if(!chip){ perror("gpiod_chip_open_by_name"); return 1; }
  struct gpiod_line* dir = gpiod_chip_get_line(chip, 13); // GPIO13
  if(!dir){ perror("gpiod_chip_get_line(13)"); return 1; }
  if(gpiod_line_request_output(dir, "motor-dir", 0) < 0){ perror("gpiod_line_request_output"); return 1; }

  // ----- PWM on GPIO12 only (pwm0) -----
  const char* PWM0_ENABLE = "/sys/class/pwm/pwmchip0/pwm0/enable";
  const char* PWM0_PERIOD = "/sys/class/pwm/pwmchip0/pwm0/period";
  const char* PWM0_DUTY   = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";

  // make sure exported/enabled (idempotent)
  wstr("/sys/class/pwm/pwmchip0/export", "0");
  wstr(PWM0_ENABLE, "0");
  const unsigned long long period_ns = 100000; // 10 kHz
  wull(PWM0_PERIOD, period_ns);
  wull(PWM0_DUTY, 0);
  wstr(PWM0_ENABLE, "1");

  // ----- PI control (gentle gains) -----
  const int    counts_per_rev = 80;  // SET: encoder CPR per channel × 4
  const double Ts = 0.01;            // 100 Hz
  double Kp = 0.2, Ki = 2.0;         // start gentle to avoid jolts
  double ui = 0.0;

  // for direction: we set GPIO13 to sign(ref_rps)
  int dir_ref = (ref_rps >= 0) ? 0 : 1;  // choose which level is "forward" for your wiring
  gpiod_line_set_value(dir, dir_ref);
  double ref_mag = fabs(ref_rps);        // controller uses magnitude only

  unsigned long long last_cnt = 0; read(enc, &last_cnt, sizeof(last_cnt));
  double t_prev = now_s();

  while(1){
    usleep((useconds_t)(Ts*1e6));
    double t = now_s(), dt = t - t_prev; t_prev = t;

    unsigned long long cnt=0;
    if(read(enc, &cnt, sizeof(cnt)) != sizeof(cnt)){ perror("read enc"); break; }
    long long dcounts = (long long)(cnt - last_cnt); last_cnt = cnt;

    double rps = (dt>0) ? ((double)dcounts / counts_per_rev) / dt : 0.0;
    double rps_mag = fabs(rps); // we regulate speed magnitude; direction fixed by GPIO13

    // PI on magnitude only; clamp 0..1
    double e = ref_mag - rps_mag;
    ui += Ki * e * dt; if(ui < 0.0) ui = 0.0; if(ui > 1.0) ui = 1.0;
    double u = Kp * e + ui; if(u < 0.0) u = 0.0; if(u > 1.0) u = 1.0;

    unsigned long long duty = (unsigned long long)(u * (double)period_ns);
    wull(PWM0_DUTY, duty);

    printf("dt=%.4f  dcounts=%lld  rps=%.3f  |ref|=%.2f  dir=%d  duty=%llu/%llu\n",
           dt, dcounts, rps, ref_mag, dir_ref, duty, period_ns);
    fflush(stdout);
  }

  wstr(PWM0_ENABLE, "0");
  gpiod_line_release(dir);
  gpiod_chip_close(chip);
  return 0;
}
