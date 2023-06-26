cmake_minimum_required(VERSION 3.5)

project(SudokuMaster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
        ./3rdParty
)

add_subdirectory(
        client
)

add_subdirectory(
        server
)
