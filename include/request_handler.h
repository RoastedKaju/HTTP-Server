#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "common.h"

/**
* Handles request
* @param client_socket The client socket
* @param request The Request string
* @param request_size The size of the request
* @return 0 on success, non-zero on failure
*/
int handle_request(SOCKET client_socket, const char* request, size_t request_size);


#endif // !REQUEST_HANDLER_H