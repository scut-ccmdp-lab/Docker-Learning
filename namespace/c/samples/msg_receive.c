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
	int msg_id = -1;
	struct msg_st data;
	long int msg_type = 0;

	// Create a message queue.
	msg_id = msgget((key_t)MSG_ID, 0666 | IPC_CREAT);
	if (msg_id == -1) {
		fprintf(stderr, "msgset failed with an error, errno = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// Get a message from queue.
	while (running) {
		if (msgrcv(msg_id, (void *)&data, BUFSIZ, msg_type, 0) == -1) {
			fprintf(stderr, "msgrcv failed with an error, errno = %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s\n", data.text);

		if (strncmp("end", data.text, 3) == 0) {
			running = 0;
		}
	}

	// Delete the message queue.
	if(msgctl(msg_id, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl failed with an error, errno = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
