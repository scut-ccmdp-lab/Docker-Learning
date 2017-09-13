/**
 * A demo for the utilization of Linux message queue, which
 * is one of the IPC method.
 *
 * @author: chenyang
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_ID 3534

struct msg_st {
	long int msg_type;
	char text[BUFSIZ];
};

int main(void) {
	int running = 1;
	struct msg_st data;
	char buffer[BUFSIZ];
	int msg_id = -1;

	// Create a message queue.
	msg_id = msgget((key_t)MSG_ID, 0666 | IPC_CREAT);
	if (msg_id == -1) {
		fprintf(stderr, "msgset failed with an error, errno = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// Get a message from queue.
	while (running) {
		// Input some text.
		printf("Enter some text:");
		fgets(buffer, BUFSIZ, stdin);
		data.msg_type = 1;
		strcpy(data.text, buffer);

		// Send a message to queue.
		if(msgsnd(msg_id, (void *)&data, BUFSIZ, 0) == -1) {
			fprintf(stderr, "msgsnd failed with an error, errno = %d\n", errno);
			exit(EXIT_SUCCESS);
		}

		if(strncmp("end", buffer, 3) == 0) {
			running = 0;
		}

		sleep(1);
	}

	return EXIT_SUCCESS;
}
