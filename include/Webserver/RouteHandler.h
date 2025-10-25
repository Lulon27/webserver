#pragma once

#include <functional>
#include <vector>

#include "Webserver/HTTPPath.h"
#include "Webserver/HTTPRequest.h"
#include "Webserver/HTTPResponse.h"

class RouteHandler
{
public:
    using CallbackFn = std::function<void(const HTTPRequest&, HTTPResponse&)>;

public:
    void addRouteCallback(const HTTPPath& route, CallbackFn callbackFn)
    {
        m_routeCallbacks.push_back({route, callbackFn});
    }

    bool handleRequest(const HTTPRequest& req, HTTPResponse& res)
    {
        for(const auto&[path, callbackFn] : m_routeCallbacks)
		{
			if(path.getPath() == "*" || req.path.rfind(path.getPath(), 0) == 0)
			{
				callbackFn(req, res);
                return true;
			}
		}
        return false;
    }

private:
    std::vector<std::pair<HTTPPath, CallbackFn>> m_routeCallbacks;
};
