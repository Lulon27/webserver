set(FILES_CPP "StringUtil.cpp" "HTTPRequest.cpp" "HTTPResponse.cpp" "HTTPServer.cpp")

add_library (libwebserver ${FILES_CPP})
target_include_directories(libwebserver PUBLIC "${PROJECT_SOURCE_DIR}/include/")

message("${PROJECT_SOURCE_DIR}/include/")
