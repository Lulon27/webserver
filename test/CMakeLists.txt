set(FILES_CPP "main.cpp")

add_executable(WebserverTest ${FILES_CPP})

include_directories(WebserverTest PUBLIC "${PROJECT_SOURCE_DIR}/include/")

target_link_libraries (WebserverTest libwebserver)
