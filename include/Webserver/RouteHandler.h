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
    
private:
    class Route
    {
    public:
        Route(const HTTPPath& path, bool serveDirectoryRoute, CallbackFn callbackFunction) : m_path(path), m_serveDirectoryRoute(serveDirectoryRoute), m_callbackFunction(callbackFunction)
        {
            m_pathParamNames.reserve(std::distance(path.getPath().begin(), path.getPath().end()));
            for(const auto& segment : path.getPath())
            {
                const auto& name = segment.string();
                if(name.rfind(":", 0) == 0)
                {
                    m_pathParamNames.push_back(std::string(std::string_view(name).substr(1)));
                }
                else
                {
                    m_pathParamNames.emplace_back();
                }
            }
        }

        bool match(const HTTPRequest& req, const HTTPResponse& res, bool matchDifferentSize, std::unordered_map<std::string, std::string>& pathParams) const
        {
            if(m_path.getPath() == "*")
			{
                return true;
			}
            size_t index = 0;
            auto itReqPath = req.path.getPath().begin();
            auto itRoutePath = m_path.getPath().begin();
            while(itReqPath != req.path.getPath().end() && itRoutePath != m_path.getPath().end())
            {

                if(!m_pathParamNames[index].empty())
                {
                    pathParams.emplace(m_pathParamNames[index], *itReqPath);
                    ++index;
                    ++itReqPath;
                    ++itRoutePath;
                    continue;
                }

                if(*itReqPath != *itRoutePath)
                {
                    return false;
                }
                ++index;
                ++itReqPath;
                ++itRoutePath;
            }
            if(!matchDifferentSize && itReqPath != req.path.getPath().end() || itRoutePath != m_path.getPath().end())
            {
                // Different number of segments
                return false;
            }
            return true;
        }

    private:
        HTTPPath m_path;
        bool m_serveDirectoryRoute;
        CallbackFn m_callbackFunction;

        // Length is the same as number of path segments.
        // Path segments without parameter are empty string_views.
        std::vector<std::string> m_pathParamNames;

        friend class RouteHandler;
    };

public:
    void addRouteCallback(const HTTPPath& route, bool serveDirectoryRoute, CallbackFn callbackFn)
    {
        m_routeCallbacks.emplace_back(route, serveDirectoryRoute ,callbackFn);
    }

    bool handleRequest(HTTPRequest& req, HTTPResponse& res) const
    {
        for(const auto& route : m_routeCallbacks)
		{
			if(route.match(req, res, route.m_serveDirectoryRoute, req.pathParams))
            {
				route.m_callbackFunction(req, res);
                return true; // Only handle the first match for now
            }
		}
        return false;
    }

    

private:
    std::vector<Route> m_routeCallbacks;
};
