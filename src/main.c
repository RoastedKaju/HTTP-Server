#include "server.h"

volatile BOOL server_running = TRUE;

BOOL WINAPI ConsoleHandler(DWORD signal)
{
	if (signal == CTRL_C_EVENT ||
		signal == CTRL_BREAK_EVENT ||
		signal == CTRL_CLOSE_EVENT ||
		signal == CTRL_SHUTDOWN_EVENT)
	{
		server_running = FALSE;
		printf("\nServer shutting down...\n");
		return TRUE;
	}

	return FALSE;
}

int main()
{
	// Setup console control handle
	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
	{
		fprintf(stderr, "Error setting console handler.\n");
		return 1;
	}

	printf("Starting HTTP server on port: %d\n", PORT);
	printf("Press CTRL+C to stop the server");

	// Initialize and start the server
	if (start_server(PORT) != 0)
	{
		fprintf(stderr, "Failed to start server.\n");
		return 1;
	}

	// Run the server until signaled to stop
	while (server_running)
	{
		server_process_request();
	}

	// Cleanup
	server_cleanup();
	printf("Server stopped.\n");

	return 0;
}