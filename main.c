#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "socket-client.h"
#include "viewer.h"

int main(int argc, char *argv[]) {
	printf("adns image grabber - client socket program\n");
	if( argc < 2 ){
		printf("usage: %s ServerAdresse\n", *argv);
		exit(EXIT_FAILURE);
	}
	
	// setup socket
	if (socket_client_init(argv[1]) == FAIL) return EXIT_FAILURE;
	if (socket_client_connect() == FAIL) return EXIT_FAILURE;
	
	// prepare output
	viewer_init(&argc, &argv);	

	while(1) {
		// request raw frame
		socket_client_send("grab", 4);
		// receive frame data
		char *frame = NULL;
		int size;
		socket_client_receive(&frame, &size);

		if (size > 1) {
			if (viewer_set_image(frame, 30,30) == EXIT_FAILURE) {
				// exit
				socket_client_send("exit", 4);
				break;
			}
		}
		viewer_update();
	}

	socket_client_close();
	
	return EXIT_SUCCESS;
}
