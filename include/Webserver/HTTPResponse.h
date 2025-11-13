#pragma once

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

class HTTPResponse
{
public:
    using StatusCode = unsigned short;

public:
    HTTPResponse();

    size_t createResponse(char* buffer, size_t bufferSize) const;
    
    StatusCode getStatusCode() const;
    void setStatusCode(StatusCode statusCode);
    const std::string& getVersion() const;
    const std::unordered_map<std::string, std::string>& getHeaders() const;
    void setContent(const char* content, size_t contentSize);
    void setContent(const std::string& text);
    std::vector<char>& getContentBuffer();

private:
	std::string m_version;
	std::unordered_map<std::string, std::string> m_headers;
    StatusCode m_statusCode;
    std::vector<char> m_content;
};
