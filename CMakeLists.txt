cmake_minimum_required(VERSION 3.10)
project(FileOrganizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${yaml-cpp_INCLUDE_DIRS}
)

add_executable(organizer 
    src/main.cpp
    src/FolderWatcher.cpp
    src/Logger.cpp  # 添加此行
)

target_link_libraries(organizer PRIVATE 
    yaml-cpp
    pthread
)

if(APPLE)
    link_directories(/opt/homebrew/lib)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")
endif()
