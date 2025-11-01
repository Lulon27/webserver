#include "Webserver/HTTPServer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
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
	std::string_view v = req.path.getPath().string();
	auto substr = v.substr(1);
	auto path = directoryPath / substr;
	
	if(std::filesystem::is_directory(path))
	{
		path /= "index.html";
	}

	try
	{
		printf("%s\n", path.c_str());
		path = std::filesystem::canonical(path);
		printf("%s\n", path.c_str());
	}
	catch(const std::filesystem::filesystem_error& e)
	{
		printf("%s\n", e.code().message().c_str());
		res.setStatusCode(404);
		return;
	}

    if(std::mismatch(path.begin(), path.end(), directoryPath.begin(), directoryPath.end()).second != directoryPath.end())
	{
		printf("Request path not in serving directory\n");
		res.setStatusCode(404);
		return;
	}
	
	auto& contentBuffer = res.getContentBuffer();
	auto fp = std::fopen(path.c_str(), "rb");
	if(!fp)
	{
		printf("fopen failed: %s\n", std::strerror(errno));
		res.setStatusCode(404);
		return;
	}
	std::fseek(fp, 0u, SEEK_END);
	auto size = std::ftell(fp);
	if(size < 0)
	{
		printf("ftell failed: %s\n", std::strerror(errno));
		res.setStatusCode(500);
		return;
	}
	contentBuffer.resize(size);
	std::fseek(fp, 0u, SEEK_SET);
	std::fread(contentBuffer.data(), 1u, contentBuffer.size(), fp);
	std::fclose(fp);

	res.setStatusCode(200);
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
			if(!m_serveDirectoryRouteHandler.handleRequest(req, res))
			{
				// No handler created a result...
				// Create default 404 response in the next step
			}
		}
		size_t responseSize = res.createResponse(buffer, bufferSize);
		if(responseSize == 0)
		{
			printf("ERROR: responseSize == 0\n");
			close(clientSocket_fd);
			continue;
		}
		n = write(clientSocket_fd, buffer, responseSize);
		if (n < 0)
		{
			printf("ERROR: n < 0\n");
			close(clientSocket_fd);
			continue;
		}
		close(clientSocket_fd);
	}
	close(socket_fd);
	delete[] buffer;
}
