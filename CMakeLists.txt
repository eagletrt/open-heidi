cmake_minimum_required(VERSION 3.16)
project(OpenHeidi)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-stdlib=libc++)

add_subdirectory(external/json)
add_subdirectory(external/cpp-httplib)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

include_directories(core/requests)

add_executable(
  OpenHeidi
  core/main.cpp
  core/server.cpp
  core/requests/device.cpp
  core/requests/openid_configuration.cpp
  core/requests/token.cpp
)
target_link_libraries(OpenHeidi PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(OpenHeidi PRIVATE ${CURL_LIBRARIES})

