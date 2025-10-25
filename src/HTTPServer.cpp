#include "Webserver/HTTPServer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Webserver/HTTPRequest.h"

static void onError(const std::string& msg)
{
	printf("Error: %s\n", msg.c_str());
	exit(1);
}

static constexpr size_t bufferSize = 1024 * 10;
static char* buffer = new char[bufferSize];

void HTTPServer::handleServeDirectoryRequest(const HTTPRequest& req, HTTPResponse& res, const std::filesystem::path& directoryPath)
{
	printf("Handle serve directory\n");
}

void HTTPServer::listen(uint16_t port)
{
    struct sockaddr_in server_addr, client_addr;


	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		onError("Failed to create socket");
	}
	printf("Socked created: %d\n", socket_fd);

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		onError("Failed to bind socket to address");
	}
	::listen(socket_fd, 5);
	socklen_t clientLength = sizeof(client_addr);
	while(true)
	{
		printf("Waiting for client to connect...\n");
		int clientSocket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &clientLength);
		if (clientSocket_fd < 0)
		{
			onError("Error on accept()");
		}
		memset(buffer, 0, bufferSize);

		printf("Reading...\n");
		int n = read(clientSocket_fd, buffer, bufferSize - 1);
		if (n < 0)
		{
			onError("Error on read()");
		}
		printf("read() returned %d\n", n);
		printf("---------- Client message ----------\n%s\n------------------------------------\n", buffer);

		HTTPResponse res;
        HTTPRequest req = HTTPRequest::parse(buffer, n);
		if(!m_routeHandler.handleRequest(req, res))
		{
			m_serveDirectoryRouteHandler.handleRequest(req, res);
		}

		size_t responseSize = res.createResponse(buffer, bufferSize);
		if(responseSize == 0)
		{
			onError("Error on write()");
		}
		n = write(clientSocket_fd, buffer, responseSize);
		if (n < 0)
		{
			onError("Error on write()");
		}
		printf("write() returned %d\n", n);

		close(clientSocket_fd);
	}
	close(socket_fd);
	delete[] buffer;
}
