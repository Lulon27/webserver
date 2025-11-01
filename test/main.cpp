#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "Webserver/HTTPServer.h"

static void handle_route(const HTTPRequest& req, HTTPResponse& res)
{
	printf("Parsed client request:\n");
	printf("- Method: %s\n", req.method.c_str());
	printf("- Path: %s\n", req.path.getPath().c_str());
	printf("- Version: %s\n", req.version.c_str());
	printf("- Query parameters:\n");
	for(auto& kv : req.queryParams)
	{
		printf("\t%s: %s\n", kv.first.c_str(), kv.second.c_str());
	}
	printf("- Headers:\n");
	for(auto& kv : req.headers)
	{
		printf("\t%s: %s\n", kv.first.c_str(), kv.second.c_str());
	}
	printf("- Path parameters:\n");
	for(auto& kv : req.pathParams)
	{
		printf("\t%s: %s\n", kv.first.data(), kv.second.c_str());
	}
	printf("\n");
	res.setStatusCode(200);
}

int main()
{
	HTTPServer server;

	server.addRouteCallback("/api/led_strip/:id", &handle_route);
	server.addServeDirectory("/", "./public");

	server.listen(1234);

	return 0;
}
