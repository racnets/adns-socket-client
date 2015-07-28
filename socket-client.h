/*
 * socket-client.h
 */

#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_
#include <stdint.h>

typedef enum {
	SUCCESS,
	FAIL
} result_t;	

result_t socket_client_close(void);
result_t socket_client_connect(void);
result_t socket_client_init(const char *addr);
result_t socket_client_receive(char **val, int *len);
result_t socket_client_send(char *val, int len);

#endif /* SOCKET_CLIENT_H_ */
