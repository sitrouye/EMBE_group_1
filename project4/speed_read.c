#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

static double now_s() {
  struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec*1e-9;
}

int main(int argc, char** argv) {
  const int counts_per_rev = 80; // <-- set yours (4× CPR)
  const double Ts = 0.05;        // 50 ms sample
  int fd = open("/dev/encoder0", O_RDONLY);
  if (fd < 0) { perror("open /dev/encoder0"); return 1; }

  // reset
  int w = open("/dev/encoder0", O_WRONLY); if (w>=0){ write(w,"reset",5); close(w); }

  long long prev = 0;
  read(fd, &prev, sizeof(prev)); // first read
  double t_prev = now_s();

  while (1) {
    usleep((useconds_t)(Ts*1e6));
    double t_now = now_s();
    long long cur = 0;
    if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) { perror("read"); break; }

    long long dcounts = cur - prev; prev = cur;
    double dt = t_now - t_prev; t_prev = t_now;

    double revs = (double)dcounts / (double)counts_per_rev;
    double rps  = (dt>0) ? revs/dt : 0.0;
    double rpm  = rps * 60.0;

    printf("dt=%.3f s, dcounts=%lld, rps=%.3f, rpm=%.1f, total=%lld\n",
           dt, dcounts, rps, rpm, cur);
    fflush(stdout);
  }
}
