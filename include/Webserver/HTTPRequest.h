#pragma once

#include "Webserver/HTTPPath.h"

#include <string>
#include <unordered_map>

class HTTPRequest
{
public:
	static HTTPRequest parse(const char* buffer, size_t n);

public:
	std::string method;
	std::string version;
	HTTPPath path;
	std::unordered_map<std::string, std::string> headers;
	std::unordered_map<std::string, std::string> queryParams;
	std::unordered_map<std::string, std::string> pathParams;
};
