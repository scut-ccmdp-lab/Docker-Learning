#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

const int NUM_THREADS = 5;

void* thread_main(void *threadid) {
    char cmd[128];
    sprintf(cmd, "echo %ld >> /sys/fs/cgroup/cpu/tinylcy/tasks", syscall(SYS_gettid));
    system(cmd);
    sprintf(cmd, "echo %ld >> /sys/fs/cgroup/cpuset/tinylcy/tasks", syscall(SYS_gettid));
    system(cmd);

    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld, pid #%ld!\n", tid, syscall(SYS_gettid));

    int a = 0;
    while(1) {
        a++;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int num_threads;
    if(argc > 1) {
        num_threads = atoi(argv[1]);
    }
    if(num_threads <= 0 || num_threads >= 100) {
        num_threads = NUM_THREADS;
    }

    mkdir("/sys/fs/cgroup/cpu/tinylcy", 755);
    system("echo 50000 > /sys/fs/cgroup/cpu/tinylcy/cpu.cfs_quota_us");

    mkdir("/sys/fs/cgroup/cpuset/tinylcy", 755);
    system("echo \"2,3\" > /sys/fs/cgroup/cpuset/tinylcy/cpuset.cpus");

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
    int rc;
    long t;
    for(t = 0; t < num_threads; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_main, (void*)t);
        if(rc) {
            fprintf(stderr, "fail to create thread");
            exit(-1);
        }
    }

    pthread_exit(NULL);
    free(threads);
    return 0;
}
