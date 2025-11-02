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

static constexpr size_t bufferSize = 1024 * 10;
static char* buffer = new char[bufferSize];

void HTTPServer::handleServeDirectoryRequest(const HTTPRequest& req, HTTPResponse& res, const std::filesystem::path& directoryPath)
{
	auto pathStr = req.path.getPath().string();
	std::string_view v = pathStr;
	auto substr = v.substr(1);
	auto path = directoryPath / substr;
	
	if(std::filesystem::is_directory(path))
	{
		path /= "index.html";
	}

	try
	{
		path = std::filesystem::canonical(path);
	}
	catch(const std::filesystem::filesystem_error& e)
	{
		res.setStatusCode(404);
		return;
	}

    if(std::mismatch(path.begin(), path.end(), directoryPath.begin(), directoryPath.end()).second != directoryPath.end())
	{
		res.setStatusCode(404);
		return;
	}
	
	auto& contentBuffer = res.getContentBuffer();
	auto fp = std::fopen(path.c_str(), "rb");
	if(!fp)
	{
		res.setStatusCode(404);
		return;
	}
	std::fseek(fp, 0u, SEEK_END);
	auto size = std::ftell(fp);
	if(size < 0)
	{
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
		throw std::runtime_error("Failed to create socket");
	}

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		throw std::runtime_error("Failed to bind socket to address");
	}
	::listen(socket_fd, 5);
	socklen_t clientLength = sizeof(client_addr);
	while(true)
	{
		int clientSocket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &clientLength);
		if (clientSocket_fd < 0)
		{
			throw std::runtime_error("Error on accept()");
		}
		memset(buffer, 0, bufferSize);

		int n = read(clientSocket_fd, buffer, bufferSize - 1);
		if (n < 0)
		{
			throw std::runtime_error("Error on read()");
		}

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
			close(clientSocket_fd);
			continue;
		}
		n = write(clientSocket_fd, buffer, responseSize);
		if (n < 0)
		{
			close(clientSocket_fd);
			continue;
		}
		close(clientSocket_fd);
	}
	close(socket_fd);
	delete[] buffer;
}
