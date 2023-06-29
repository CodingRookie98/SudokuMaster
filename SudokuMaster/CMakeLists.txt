cmake_minimum_required(VERSION 3.5)

project(SudokuMaster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置默认的源目录
if (NOT DEFINED CMAKE_SOURCE_DIR)
    set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux系统
    # 在这里添加针对Linux的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/Debug-Linux)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/Release-Linux)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/minSizeRe-Linux)
    # 设置默认的构建目录
    if (NOT DEFINED CMAKE_BINARY_DIR)
        set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build-linux)
    endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows系统
    # 在这里添加针对Windows的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/Debug-Win)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/Release-Win)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/minSizeRe-Win)
    # 设置默认的构建目录
    if (NOT DEFINED CMAKE_BINARY_DIR)
        set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build-win)
    endif ()
endif ()

message(STATUS "CMake Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMake Binary Directory: ${CMAKE_BINARY_DIR}")

include_directories(
        ./3rdParty/include
)

add_subdirectory(
        client
)

add_subdirectory(
        server
)
