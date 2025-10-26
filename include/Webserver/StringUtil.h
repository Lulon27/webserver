#pragma once

#include <string>
#include <cstring>
#include <cstdarg>

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

class StringWriter
{
public:
    StringWriter(char* string, size_t stringSize) : m_start(string), m_current(string), m_stringSize(stringSize)
    {
        
    }

    void write(const char* source);
    void writeFormat(const char* format, ...);
    void copy(const char* source, size_t length);

    bool isEndOfBuffer() const
    {
        return m_current == nullptr || getRelativePosition() >= m_stringSize;
    }

    bool isError() const
    {
        return m_error;
    }

    const char* getPosition() const
    {
        return m_current;
    }

    size_t getRelativePosition() const
    {
        return m_current - m_start;
    }

    size_t getRemaining() const
    {
        return m_stringSize - getRelativePosition();
    }

private:
    char* m_start;
    char* m_current;
    const size_t m_stringSize;
    bool m_error = false;
};
