#include "Webserver/HTTPRequest.h"

#include "Webserver/StringUtil.h"

HTTPRequest HTTPRequest::parse(const char* buffer, size_t n)
{
    HTTPRequest msg;
    if(n == 0)
    {
        return msg;
    }

    StringParser parser(buffer, n);

    msg.method = parser.untilChar(' ');

    parser.skipOne();
    msg.path = parser.untilChar('?');
    if(parser.isError())
    {
        msg.path = parser.untilChar(' ');
    }
    else
    {
        while(!parser.isEndOfBuffer() && !parser.isError() && *parser.getPosition() != ' ' && msg.queryParams.size() < 32)
        {
            parser.skipOne();
            std::string paramName = parser.untilChar('=');
            parser.skipOne();
            std::string paramValue = parser.untilChar('&');
            if(parser.isError())
            {
                paramValue = parser.untilChar(' ');
            }
            msg.queryParams[paramName] = paramValue;
            if(msg.queryParams.size() > 31)
            {
                break;
            }
        }
    }

    parser.skipOne();
    msg.version = parser.untilChar('\r');

    if(parser.next() != '\n')
    {
        return msg;
    }

    parser.skipOne();

    while(!parser.isEndOfBuffer() && !parser.isError() && *parser.getPosition() != '\r' && msg.headers.size() < 32)
    {
        std::string headerName = parser.untilChar(':');

        parser.skipOne();
        if(parser.next() == ' ')
        {
            parser.skipOne();
        }

        std::string headerValue = parser.untilChar('\r');
        msg.headers[headerName] = headerValue;

        if(parser.next() != '\n')
        {
            return msg;
        }
        parser.skipOne();
    }

    return msg;
}
