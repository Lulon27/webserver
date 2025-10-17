#pragma once

#include <string>
#include <unordered_map>

class HTTPMessage
{
public:
	static HTTPMessage parse(const char* buffer, size_t n);

	std::string m_method;
	std::string m_version;
	std::string m_path;
	std::unordered_map<std::string, std::string> m_headers;
	std::unordered_map<std::string, std::string> m_queryParams;
};
