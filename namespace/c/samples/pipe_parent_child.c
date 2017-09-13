/**
 * A demo for the utilization of Linux pipe.
 *
 * @author: chenyang
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
	int pipefd[2];
	pid_t child_pid;
	char buf[BUFSIZ];

	if(pipe(pipefd) == -1) {
		perror("pipe failed.");
		exit(EXIT_FAILURE);
	}

	if((child_pid = fork()) == -1) {
		perror("fork failed.");
		exit(EXIT_FAILURE);
	}

	if(child_pid == 0) {
		// printf("Child pid is %d\n", getpid());
		if(read(pipefd[0], buf, BUFSIZ) == -1) {
			perror("read failed.");
			exit(EXIT_FAILURE);
		}
		printf("Child have read: %s", buf);

		bzero(buf, BUFSIZ);

		sleep(2);

		snprintf(buf, BUFSIZ, "Message from child: My pid is %d\n", getpid());
		if(write(pipefd[1], buf, strlen(buf)) == -1) {
			perror("write failed.");
			exit(EXIT_FAILURE);
		}
		printf("Child have writed: %s", buf);

	} else {
		// printf("Parent pid is %d\n", getpid());
		snprintf(buf, BUFSIZ, "Message from parent: My pid is %d\n", getpid());
		if(write(pipefd[1], buf, strlen(buf)) == -1) {
			perror("write failed.");
			exit(EXIT_FAILURE);
		}
		printf("Parent have writed: %s", buf);

		sleep(2);

		bzero(buf, BUFSIZ);

		if(read(pipefd[0], buf, BUFSIZ) == -1) {
			perror("read failed.");
			exit(EXIT_FAILURE);
		}
		printf("Parent have read: %s", buf);


	}

	return EXIT_SUCCESS;
}