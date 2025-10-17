#pragma once

#include <filesystem>

class HTTPPath
{
public:
    HTTPPath(const std::filesystem::path& path) : m_path(path)
    {
        if(!path.empty() && path.is_relative())
        {
            throw std::runtime_error("http path must be absolute");
        }
    }

    HTTPPath(const std::string& path = "") : HTTPPath(std::filesystem::path(path))
    {

    }

    HTTPPath(const char* path) : HTTPPath(std::filesystem::path(path))
    {

    }

    const std::filesystem::path& getPath() const
    {
        return m_path;
    }

private:
    std::filesystem::path m_path;
};
