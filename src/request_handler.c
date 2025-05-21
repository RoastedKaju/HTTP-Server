#include "request_handler.h"

const char* get_mime_type(const char* path)
{
	const char* dot = strrchr(path, '.');
	if (dot)
	{
		for (int i = 0; mime_types[i].extension != NULL; i++)
		{
			if (strcmp(dot, mime_types[i].extension) == 0)
			{
				return mime_types[i].mime;
			}
		}
	}
	return mime_types[sizeof(mime_types) / sizeof(mime_types[0]) - 1].mime;
}

void send_headers(SOCKET client_socket, const char* status, const char* content_type, size_t content_length)
{
	char headers[BUFFER_SIZE];
	char date_str[100];
	time_t now = time(NULL);
	struct tm* tm_info = gmtime(&now);

	strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S GMT", tm_info);

	// Format the headers
	int len = snprintf(headers, BUFFER_SIZE,
		"HTTP/1.1 %s \r\n"
		"Date: %s\r\n"
		"Server: SimpleHTTP/1.0\r\n"
		"Content-Length: %zu\r\n"
		"Content-Type: %s\r\n"
		"COnnection: close\r\n"
		"\r\n",
		status, date_str, content_length, content_type);

	send(client_socket, headers, (int)len, 0);
}

void send_text_response(SOCKET client_socket, const char* status, const char* content_type, const char* body)
{
	send_headers(client_socket, status, content_type, strlen(body));
	send(client_socket, body, (int)strlen(body), 0);
}

void send_404(SOCKET client_socket)
{
	const char* body = "<html><head><title>404 Not Found</title></head>"
		"<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body></html>";
	send_text_response(client_socket, HTTP_NOT_FOUND, "text/html", body);
}

void send_400(SOCKET client_socket)
{
	const char* body = "<html><head><title>400 Bad Request</title></head>"
		"<body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>";
	send_text_response(client_socket, HTTP_BAD_REQUEST, "text/html", body);
}

void send_405(SOCKET client_socket)
{
	const char* body = "<html><head><title>405 Method Not Allowed</title></head>"
		"<body><h1>405 Method Not Allowed</h1><p>The method specified in the request is not allowed for the resource identified.</p></body></html>";
	send_text_response(client_socket, HTTP_METHOD_NOT_ALLOWED, "text/html", body);
}

void send_500(SOCKET client_socket)
{
	const char* body = "<html><head><title>500 Internal Server Error</title></head>"
		"<body><h1>500 Internal Server Error</h1><p>The server encountered an internal error and was unable to complete your request.</p></body></html>";
	send_text_response(client_socket, HTTP_INTERNAL_SERVER_ERROR, "text/html", body);
}

int handle_get_request(SOCKET client_socket, const char* path)
{
    char file_path[MAX_PATH_LENGTH];

    // Build the file path with Windows path separators
    snprintf(file_path, sizeof(file_path), "%s%s", ROOT_DIRECTORY, path);

    // Replace forward slashes with backslashes for Windows
    for (char* p = file_path; *p; p++) {
        if (*p == '/') *p = '\\';
    }

    // If path ends with '\', append 'index.html'
    if (file_path[strlen(file_path) - 1] == '\\') {
        strncat(file_path, "index.html", sizeof(file_path) - strlen(file_path) - 1);
    }

    // Open the file in binary mode for Windows
    int file_fd = _open(file_path, _O_RDONLY | _O_BINARY);
    if (file_fd < 0) {
        printf("File not found: %s\n", file_path);
        send_404(client_socket);
        return -1;
    }

    // Get file size
    struct _stat file_stat;
    if (_fstat(file_fd, &file_stat) < 0) {
        _close(file_fd);
        send_500(client_socket);
        return -1;
    }

    // Get content type based on file extension
    const char* content_type = get_mime_type(file_path);

    // Send headers
    send_headers(client_socket, HTTP_OK, content_type, file_stat.st_size);

    // Send file content
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = _read(file_fd, buffer, sizeof(buffer))) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    _close(file_fd);
    return 0;
}

int handle_request(SOCKET client_socket, const char* request, size_t request_size)
{
    // Simple request parsing
    char method[16] = { 0 };
    char path[MAX_PATH_LENGTH] = { 0 };
    char protocol[16] = { 0 };

    // Parse the request line
    if (sscanf(request, "%15s %2047s %15s", method, path, protocol) != 3) {
        send_400(client_socket);
        return -1;
    }

    printf("Received request: %s %s %s\n", method, path, protocol);

    // Only handle HTTP/1.0 and HTTP/1.1
    if (strcmp(protocol, "HTTP/1.0") != 0 && strcmp(protocol, "HTTP/1.1") != 0) {
        send_400(client_socket);
        return -1;
    }

    // We only support GET method for now
    if (strcmp(method, "GET") == 0) {
        return handle_get_request(client_socket, path);
    }
    else {
        send_405(client_socket);
        return -1;
    }
}
