cmake_minimum_required(VERSION 3.16)
project(OpenHeidi)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/json)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(OpenHeidi core/main.cpp)
target_link_libraries(OpenHeidi PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(OpenHeidi PRIVATE ${CURL_LIBRARIES})

