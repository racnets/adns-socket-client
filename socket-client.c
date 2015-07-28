#include <sys/types.h>
#include <sys/socket.h>	// socket(), connect(), send(), recv()
#include <netinet/in.h>	// sockaddr_in
#include <arpa/inet.h>	// htons(), inet_aton()
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "socket-client.h"

#define BUF 1024

int created_socket;
struct sockaddr_in address = {0};
char *buffer;

result_t socket_client_init(const char *addr) {
	// create socket
	if ((created_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
		printf ("socket successfully created\n");
	} else {
		printf ("error creating socket\n");
		return FAIL;
	}
	
	// prepare connection
	address.sin_family = AF_INET;
	address.sin_port = htons(15000);
	inet_aton(addr, &address.sin_addr);
	
	buffer = malloc (BUF);

	return SUCCESS;
}

result_t socket_client_connect(void) {
	if (connect(created_socket, (struct sockaddr *) &address, sizeof(address)) == 0) {
		printf ("established connection to server(%s)\n", inet_ntoa(address.sin_addr));
		return SUCCESS;
	} else {
		printf ("connection to server(%s) failed\n", inet_ntoa(address.sin_addr));
		return FAIL;
	}
}

result_t socket_client_send(char *val, int len) {
	// send
	send(created_socket, val, len, 0);

	return SUCCESS;
}

result_t socket_client_receive(char **val, int *len) {
	ssize_t size;
	size = recv(created_socket, buffer, BUF-1, 0);
	if( size > 0) {
		buffer[size] = '\0';
		//~ printf ("%d bytes received\n" ,size);
	}
	
	*len = size;
	*val = malloc(size);
	memcpy(*val, buffer, size);

	//~ int i;
	//~ for (i = 0; i < size; i++) {
		//~ if (!(i % 8)) puts("");
		//~ printf("%.2X ", (*val)[i]);
	//~ }
	//~ printf("\n");
	
	return SUCCESS;
}

result_t socket_client_close(void) {
	// close socket
	close (created_socket);
	
	return SUCCESS;
}
