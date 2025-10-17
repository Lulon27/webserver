#pragma once

#include <string>
#include <cstring>

class StringParser
{
public:
    StringParser(const char* string, size_t stringSize) : m_stringStart(string), m_current(string), m_stringSize(stringSize)
    {
        
    }

    std::string untilChar(char c);
    void skipOne();
    char next();

    bool isEndOfBuffer() const
    {
        return m_current == nullptr || m_current - m_stringStart >= m_stringSize || *m_current == '\0';
    }

    bool isError() const
    {
        return m_error;
    }

    const char* getPosition() const
    {
        return m_current;
    }

private:
    const char* m_stringStart;
    const char* m_current;
    const size_t m_stringSize;
    bool m_error = false;
};
