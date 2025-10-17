#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>

#include "Webserver/HTTPRequest.h"
#include "Webserver/HTTPPath.h"

class HTTPServer
{
public:
    using RouteCallbackFn = std::function<void(const HTTPMessage&)>;

public:
    void listen(uint16_t port);
    void setServeDirectory(const HTTPPath& route, const std::filesystem::path& directoryPath)
    {
        m_serveDirectory = directoryPath;
        m_serveDirectoryRoute = route;
    }

    void addRouteCallback(const HTTPPath& route, RouteCallbackFn callbackFn)
    {
        m_routeCallbacks.push_back({route, callbackFn});
    }

private:
    std::filesystem::path m_serveDirectory;
    HTTPPath m_serveDirectoryRoute;
    std::vector<std::pair<HTTPPath, RouteCallbackFn>> m_routeCallbacks;
};
