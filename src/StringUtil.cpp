#include "Webserver/StringUtil.h"

#include <cstdio>

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

void StringWriter::write(const char* sourceCurrent)
{
	while(*sourceCurrent != '\0' && !isEndOfBuffer())
	{
		*m_current = *sourceCurrent;
		sourceCurrent++;
		m_current++;
	}
}

void StringWriter::writeFormat(const char* format, ...)
{
	va_list va;
	if(isEndOfBuffer())
	{
		return;
	}
	size_t remaining = getRemaining();
    va_start(va, format);
	int numberWouldHaveBeenWritten = vsnprintf(m_current, remaining, format, va);
    va_end(va);
	if(numberWouldHaveBeenWritten < 0)
	{
		m_error = true;
		return;
	}
	m_current += numberWouldHaveBeenWritten;
	
}

void StringWriter::copy(const char* source, size_t length)
{
	if(isEndOfBuffer())
	{
		return;
	}
	size_t realLength = std::min(length, getRemaining());
	memcpy(m_current, source, realLength);
	m_current += realLength;
}
