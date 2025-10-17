#include "Webserver/StringUtil.h"

std::string StringParser::untilChar(char c)
{
	if(isEndOfBuffer())
	{
		return "";
	}
	const char* start = m_current;
	m_current = strchr(m_current, c);
	if(!m_current)
	{
		m_current = start;
		m_error = true;
		return "";
	}
	m_error = false;
	return std::string(start, m_current - start);
}

void StringParser::skipOne()
{
	if(isEndOfBuffer())
	{
		return;
	}
	m_error = false;
	m_current++;
}

char StringParser::next()
{
	if(isEndOfBuffer())
	{
		return 0;
	}
	m_error = false;
	m_current++;
	if(isEndOfBuffer())
	{
		return 0;
	}
	return *m_current;
}
