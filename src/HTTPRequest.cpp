#include "Webserver/HTTPRequest.h"

#include "Webserver/StringUtil.h"

HTTPMessage HTTPMessage::parse(const char* buffer, size_t n)
{
    HTTPMessage msg;
    if(n == 0)
    {
        return msg;
    }

    StringParser parser(buffer, n);

    msg.m_method = parser.untilChar(' ');

    parser.skipOne();
    msg.m_path = parser.untilChar('?');
    if(parser.isError())
    {
        msg.m_path = parser.untilChar(' ');
    }
    else
    {
        while(!parser.isEndOfBuffer() && !parser.isError() && *parser.getPosition() != ' ' && msg.m_queryParams.size() < 32)
        {
            parser.skipOne();
            std::string paramName = parser.untilChar('=');
            parser.skipOne();
            std::string paramValue = parser.untilChar('&');
            if(parser.isError())
            {
                paramValue = parser.untilChar(' ');
            }
            msg.m_queryParams[paramName] = paramValue;
            if(msg.m_queryParams.size() > 31)
            {
                break;
            }
        }
    }

    parser.skipOne();
    msg.m_version = parser.untilChar('\r');

    if(parser.next() != '\n')
    {
        return msg;
    }

    parser.skipOne();

    while(!parser.isEndOfBuffer() && !parser.isError() && *parser.getPosition() != '\r' && msg.m_headers.size() < 32)
    {
        std::string headerName = parser.untilChar(':');

        parser.skipOne();
        if(parser.next() == ' ')
        {
            parser.skipOne();
        }

        std::string headerValue = parser.untilChar('\r');
        msg.m_headers[headerName] = headerValue;

        if(parser.next() != '\n')
        {
            return msg;
        }
        parser.skipOne();
    }

    return msg;
}
