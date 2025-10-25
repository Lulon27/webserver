#pragma once

#include <cstring>
#include <string>
#include <unordered_map>

class HTTPResponse
{
public:
    using StatusCode = unsigned short;

public:
    HTTPResponse();

    size_t createResponse(char* buffer, size_t bufferSize) const;

public:
	std::string version;
	std::unordered_map<std::string, std::string> headers;
    StatusCode statusCode;
};
