#pragma once

#include <string>
#include <unordered_map>

class HTTPRequest
{
public:
	static HTTPRequest parse(const char* buffer, size_t n);

public:
	std::string method;
	std::string version;
	std::string path;
	std::unordered_map<std::string, std::string> headers;
	std::unordered_map<std::string, std::string> queryParams;
};
