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

static void handleServeDirectoryRequest(const HTTPMessage& req)
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

		HTTPMessage msg = HTTPMessage::parse(buffer, n);

        if(!m_serveDirectory.empty() && msg.m_path.rfind(m_serveDirectory, 0) == 0)
        {
            handleServeDirectoryRequest(msg);
        }
        else
        {
            for(const auto&[path, callbackFn] : m_routeCallbacks)
            {
                if(path.getPath() == "*" || msg.m_path.rfind(path.getPath(), 0) == 0)
                {
                    callbackFn(msg);
                }
            }
        }

		std::string response = "HTTP/1.1 200 OK\njeff: hello\n\nloooooooooooooool";
		n = write(clientSocket_fd, response.c_str(), response.size());
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
