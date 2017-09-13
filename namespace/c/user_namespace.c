#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/capability.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024*1024)

static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};

int pipefd[2];

void set_uid_map(pid_t pid, int inside_id, int outside_id, int len) {
    char file[256];
    FILE *mapfd = NULL;

    sprintf(file, "/proc/%d/uid_map", pid);
    mapfd = fopen(file, "w");
    if(NULL == mapfd) {
        perror("open file error");
        exit(-1);
    }

    fprintf(mapfd, "%d %d %d", inside_id, outside_id, len);
    fclose(mapfd);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int len) {
    char file[256];
    FILE *mapfd = NULL;

    sprintf(file, "/proc/%d/gid_map", pid);
    mapfd = fopen(file, "w");
    if(NULL == mapfd) {
        perror("open file error");
        exit(-1);
    }

    fprintf(mapfd, "%d %d %d", inside_id, outside_id, len);
    // printf("Success set gid map.\n");
    fclose(mapfd);
}

int container_main(void *args) {
    printf("Container PID = [%d] - inside the container\n", getpid());
    printf("Container eUID = %d, eGID = %d, UID = %d, GID = %d\n"
            , geteuid(), getegid(), getuid(), getgid());
    
    char ch;
    close(pipefd[1]);
    read(pipefd[0], &ch, 1);

    execv(container_args[0], container_args);
    printf("Something is wrong!\n");
    return -1;
}

int main(void) {
    const int gid = getgid(), uid = getuid();
    printf("Parent: eUID = %d, eGID = %d, UID = %d, GID = %d\n"
            , geteuid(), getegid(), getuid(), getgid());

    pipe(pipefd);

    printf("Parent: start a container.\n");

    int container_pid = clone(container_main, container_stack + STACK_SIZE
            , CLONE_NEWUSER | SIGCHLD, NULL);

    set_uid_map(container_pid, 0, uid, 1);
    set_gid_map(container_pid, 0, gid, 1);

    printf("Parent: user/group mapping done.\n");

    close(pipefd[1]);

    waitpid(container_pid, NULL, 0);
    printf("Parent: container stopped.\n");
    return 0;
}
