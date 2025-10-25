#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gpiod.h>

static double now_s(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec+ts.tv_nsec*1e-9;
}

static int write_str(const char*p,const char*s){
    int fd=open(p,O_WRONLY);
    if(fd<0)return -1;
    write(fd,s,strlen(s));
    close(fd);
    return 0;
}

static int write_ull(const char*p,unsigned long long v){
    char b[32];
    snprintf(b,sizeof(b),"%llu",v);
    return write_str(p,b);
}

int main(int argc,char**argv){
    if(argc<2)return 1;
    double ref_rps=atof(argv[1]);
    int w=open("/dev/encoder0",O_WRONLY);
    if(w>=0){write(w,"reset",5);close(w);}
    int enc=open("/dev/encoder0",O_RDONLY);
    if(enc<0)return 1;

    struct gpiod_chip*chip=gpiod_chip_open_by_name("gpiochip0");
    struct gpiod_line*dir=gpiod_chip_get_line(chip,13);
    gpiod_line_request_output(dir,"controller-dir",0);
    int dir_level=(ref_rps>=0)?0:1;
    gpiod_line_set_value(dir,dir_level);
    double ref_mag=fabs(ref_rps);

    const char*PWM0_ENABLE="/sys/class/pwm/pwmchip0/pwm0/enable";
    const char*PWM0_PERIOD="/sys/class/pwm/pwmchip0/pwm0/period";
    const char*PWM0_DUTY="/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
    write_str("/sys/class/pwm/pwmchip0/export","0");
    write_str(PWM0_ENABLE,"0");
    const unsigned long long period_ns=100000;
    write_ull(PWM0_PERIOD,period_ns);
    write_ull(PWM0_DUTY,0);
    write_str(PWM0_ENABLE,"1");

    const int counts_per_rev=80;
    const double Ts_usleep=0.01;
    double Kp=0.2,Ki=2.0,ui=0.0;
    unsigned long long last_cnt=0;
    read(enc,&last_cnt,sizeof(last_cnt));
    double t_prev=now_s();

    while(1){
        usleep((useconds_t)(Ts_usleep*1e6));
        double t_now=now_s(),dt=t_now-t_prev;
        t_prev=t_now;
        unsigned long long cnt=0;
        if(read(enc,&cnt,sizeof(cnt))!=(ssize_t)sizeof(cnt))break;
        long long dcounts=(long long)(cnt-last_cnt);
        last_cnt=cnt;
        double revs=(double)dcounts/(double)counts_per_rev;
        double rps=(dt>0)?revs/dt:0;
        double rps_mag=fabs(rps);
        double e=ref_mag-rps_mag;
        ui+=Ki*e*dt;
        if(ui<0)ui=0;if(ui>1)ui=1;
        double u=Kp*e+ui;
        if(u<0)u=0;if(u>1)u=1;
        unsigned long long duty=(unsigned long long)(u*period_ns);
        write_ull(PWM0_DUTY,duty);
        printf("dt=%.4f dcounts=%lld rps=%.3f ref=%.2f dir=%d duty=%llu/%llu\n",
               dt,dcounts,rps,ref_mag,dir_level,duty,period_ns);
        fflush(stdout);
    }
    write_str(PWM0_ENABLE,"0");
    gpiod_line_release(dir);
    gpiod_chip_close(chip);
    close(enc);
    return 0;
}
