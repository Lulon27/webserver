#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

#include "Webserver/RouteHandler.h"

class HTTPServer
{
public:
    void listen(uint16_t port);

    void addServeDirectory(const HTTPPath& route, const std::filesystem::path& directoryPath)
    {
        m_serveDirectoryRouteHandler.addRouteCallback(route, std::bind(handleServeDirectoryRequest, std::placeholders::_1, std::placeholders::_2, directoryPath));
    }

    void addRouteCallback(const HTTPPath& route, RouteHandler::CallbackFn callbackFn)
    {
        m_routeHandler.addRouteCallback(route, callbackFn);
    }

private:
    static void handleServeDirectoryRequest(const HTTPRequest& req, HTTPResponse& res, const std::filesystem::path& directoryPath);

private:
    RouteHandler m_routeHandler;
    RouteHandler m_serveDirectoryRouteHandler;
};
