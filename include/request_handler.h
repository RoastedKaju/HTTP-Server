#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "common.h"

// MIME types for different file extensions
struct mime_type
{
    const char* extension;
    const char* mime;
};

static const struct mime_type mime_types[] = {
    {".html",   "text/html"},
    {".htm",    "text/html"},
    {".css",    "text/css"},
    {".js",     "application/javascript"},
    {".json",   "application/json"},
    {".jpg",    "image/jpeg"},
    {".jpeg",   "image/jpeg"},
    {".png",    "image/png"},
    {".gif",    "image/gif"},
    {".ico",    "image/x-icon"},
    {".txt",    "text/plain"},
    {".pdf",    "application/pdf"},
    {NULL,      "application/octet-stream"}  // Default mime type
};

// Get the MIME type for a given file path
static const char* get_mime_type(const char* path);

/**
 * Send HTTP response headers
 */
static void send_headers(SOCKET client_socket, const char* status, const char* content_type, size_t content_length);

/**
 * Send a simple text response
 */
static void send_text_response(SOCKET client_socket, const char* status, const char* content_type, const char* body);

/**
 * Send a 404 Not Found response
 */
static void send_404(SOCKET client_socket);

/**
 * Send a 400 Bad Request response
 */
static void send_400(SOCKET client_socket);

/**
 * Send a 405 Method Not Allowed response
 */
static void send_405(SOCKET client_socket);

/**
 * Send a 500 Internal Server Error response
 */
static void send_500(SOCKET client_socket);

/**
 * Handle a GET request for a file
 */
static int handle_get_request(SOCKET client_socket, const char* path);

/**
* Handles request
* @param client_socket The client socket
* @param request The Request string
* @param request_size The size of the request
* @return 0 on success, non-zero on failure
*/
int handle_request(SOCKET client_socket, const char* request, size_t request_size);


#endif // !REQUEST_HANDLER_H