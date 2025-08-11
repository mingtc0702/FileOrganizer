cmake_minimum_required(VERSION 3.10)
project(FileOrganizer)

set(CMAKE_CXX_STANDARD 17)

# 查找yaml-cpp库
find_package(yaml-cpp REQUIRED)

add_executable(organizer main.cpp)
target_link_libraries(organizer PRIVATE yaml-cpp)
