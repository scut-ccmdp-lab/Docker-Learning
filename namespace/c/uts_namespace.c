/**
 * 注意：运行需要root权限
 * @author: chenyang
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024*1024)

// 供clone使用的函数栈，1M
static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL
};

int container_main(void *args) {
    printf("Container - inside the container.\n");
    // 设置hostname
    sethostname("tinylcy", 7);
    // 执行一个shell，用于观察进程空间内的资源是否被隔离了
    execv(container_args[0], container_args);
    printf("Somethinf wrong!\n");
    return -1;
}

int main(void) {
    printf("Parent - start a container.\n");
    // 调用clone函数，传入一个函数和栈空间的指针
    // 启用CLONE_NEWUTS Namespace隔离
    int container_pid = clone(container_main, container_stack + STACK_SIZE, SIGCHLD | CLONE_NEWUTS, NULL);
    // 等待子进程结束
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped.\n");

    return 0;
}
