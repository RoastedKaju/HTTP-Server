#ifndef SERVER_H
#define SERVER_H

#include "common.h"

static SOCKET server_socket = INVALID_SOCKET;
static struct sockaddr_in server_address;

/**
* Start Http server
* @param port The port number to listen on
* @return 0 on success, non-zero on failure
*/
int start_server(int port);

/**
* Process incoming HTTP requests
* @return 0 on success, non-zero on failure
*/
int server_process_request(void);

/**
* Clean up the server resources
*/
void server_cleanup(void);

#endif // !SERVER_H
