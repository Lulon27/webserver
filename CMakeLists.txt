cmake_minimum_required(VERSION 4.0)

project(Webserver)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("src")

option(WEBSERVER_BUILD_TESTING OFF)
if (WEBSERVER_BUILD_TESTING)
    add_subdirectory("test")
endif()
