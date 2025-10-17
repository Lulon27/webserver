#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "Webserver/HTTPServer.h"

static void handle_route(const HTTPMessage& req)
{
	printf("Parsed client request:\n");
	printf("- Method: %s\n", req.m_method.c_str());
	printf("- Path: %s\n", req.m_path.c_str());
	printf("- Version: %s\n", req.m_version.c_str());
	printf("- Query parameters:\n");
	for(auto& kv : req.m_queryParams)
	{
		printf("\t%s: %s\n", kv.first.c_str(), kv.second.c_str());
	}
	printf("- Headers:\n");
	for(auto& kv : req.m_headers)
	{
		printf("\t%s: %s\n", kv.first.c_str(), kv.second.c_str());
	}
	printf("\n");
}


int main()
{
	HTTPServer server;
	server.addRouteCallback("/led_strip", &handle_route);

	server.listen(1234);

	return 0;
}
