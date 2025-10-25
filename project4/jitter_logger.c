#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static double now_s(void){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec+ts.tv_nsec*1e-9;
}

int main(int argc,char**argv){
    double Ts=(argc>=2)?atof(argv[1]):0.01;
    int N=(argc>=3)?atoi(argv[2]):5000;
    FILE*f=fopen("jitter.csv","w");
    if(!f)return 1;
    fprintf(f,"dt_s\n");
    double t_prev=now_s();
    for(int i=0;i<N;i++){
        usleep((useconds_t)(Ts*1e6));
        double t=now_s();
        double dt=t-t_prev;
        t_prev=t;
        fprintf(f,"%.9f\n",dt);
    }
    fclose(f);
    return 0;
}
