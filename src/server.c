#include "request_handler.h"
#include "server.h"

#pragma comment(lib, "ws2_32.lib")

int start_server(int port)
{
	// Init Winsock
	WSADATA wsa_data;
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		printf("WSAStartup failed: %d\n", result);
		return -1;
	}

	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET)
	{
		printf("Failed to create socket: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// Set socket options
	char opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == SOCKET_ERROR)
	{
		printf("setsocketopt failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// Prepare the sockaddr_in structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);

	// Bind the socket
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		printf("Bind failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// Listen
	if (listen(server_socket, BACKLOG) == SOCKET_ERROR)
	{
		printf("Listen failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// Set socket to non-blocking mode
	u_long mode = 1;
	if (ioctlsocket(server_socket, FIONBIO, &mode) != NO_ERROR)
	{
		printf("ioctlsocket failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	return 0;
}

int server_process_request(void)
{
	struct sockaddr_in client_address;
	int client_len = sizeof(client_address);
	char buffer[BUFFER_SIZE];

	// Accept connection (non-blocking)
	SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);

	if (client_socket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			// No pending connections
			Sleep(10);
			return 0;
		}
		else
		{
			printf("Accept failed: %d\n", error);
			return -1;
		}
	}

	// Get client info
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
	printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_address.sin_port));

	// Recieve request
	int recv_size = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

	if (recv_size > 0)
	{
		buffer[recv_size] = '\0';

		// Handle request and send response
		handle_request(client_socket, buffer, recv_size);
	}
	else if (recv_size == 0)
	{
		printf("Client disconnected\n");
	}
	else
	{
		printf("recv failed: %d\n", WSAGetLastError());
	}

	return 0;
}

void server_cleanup(void)
{
	if (server_socket != INVALID_SOCKET)
	{
		closesocket(server_socket);
		server_socket = INVALID_SOCKET;
		WSACleanup();
	}
}
