
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
		
		/*ssize_t bytesRead = read(server, &req, sizeif(req));
		if (bytesRead <= 0) {
			//retry if read fails or returns 0
			continue;
		}*/
		if(read(server, &req, sizeof(req)) <= 0){
			continue; // ignore empty or invaild reads
		}

		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);
		fflush(stdout);

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		int targetFIFO = open(req.target, O_WRONLY);
		//check is target matches username
		//throw error if target doesn't open
		if(targetFIFO < 0) {
			//perror("Error opening target FIFO");
			fprintf(stderr, "Error: Cannot open FIFO for target user '%s'.\n", req.target);
			continue;
		}

		//write message and test if it writes
		if (write(targetFIFO, &req, sizeof(req)) < 0) {
			perror("Error writing to target FIFO");
			close(targetFIFO);
			continue;
				
		}

		// close target FIFO after writing the message
		close(targetFIFO);


	}
	close(server);
	close(dummyfd);
	return 0;
}

