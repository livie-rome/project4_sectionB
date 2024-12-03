
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO
		ssize_t bytesRead = read(server, &req, sizeif(req));
		if (bytesRead <= 0) {
			//retry if read fails or returns 0
			continue;
		}

		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		char targetFIFO[100];
		//check is target matches username
		snprintf(targetFIFO, sizeof(targetFIFO), "%s", req.target);
		target = open(targetFIFO, O_WRONLY);
		//throw error if target doesn't open
		if(target < 0) {
			perror("Error opening target FIFO");
			continue;
		}

		//send message
		write(target, &req, sizeof(req));
		// close target FIFO after writing the message
		close(target);


	}
	close(server);
	close(dummyfd);
	return 0;
}

