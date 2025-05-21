#ifndef COMMON_H
#define COMMON_H

#define PORT 8080
#define BUFFER_SIZE 8192
#define BACKLOG 10

#define MAX_PATH_LENGTH 2048
#define ROOT_DIRECTORY ".\\www"  // Document root directory for Windows

// HTTP status codes and messages
#define HTTP_OK "200 OK"
#define HTTP_NOT_FOUND "404 Not Found"
#define HTTP_BAD_REQUEST "400 Bad Request"
#define HTTP_METHOD_NOT_ALLOWED "405 Method Not Allowed"
#define HTTP_INTERNAL_SERVER_ERROR "500 Internal Server Error"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>


#endif // !COMMON_H
