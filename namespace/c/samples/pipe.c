/**
 * A demo for the utilization of Linux pipe.
 *
 * @author: chenyang
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STRING "hello pipe"

int main(void) {
	int pipefd[2];
	char buf[BUFSIZ];

	if(pipe(pipefd) == -1) {
		perror("pipe failed.");
		exit(EXIT_FAILURE);
	}

	if(write(pipefd[1], STRING, strlen(STRING)) == -1) {
		perror("write failed.");
		exit(EXIT_FAILURE);
	}

	if(read(pipefd[0], buf, BUFSIZ) == -1) {
		perror("read failed.");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", buf);

	return EXIT_SUCCESS;
}