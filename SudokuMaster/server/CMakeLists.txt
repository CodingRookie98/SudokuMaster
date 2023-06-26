set(projectName "SudokuMaster_server")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux系统
    # 在这里添加针对Linux的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-Linux)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-Linux)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/minSizeRe-Linux)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows系统
    # 在这里添加针对Windows的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-Win)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-Win)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/minSizeRe-Win)
endif ()

# 遍历所有.cpp、.h、.ui文件
file(GLOB_RECURSE cpp_h_ui_path
        ./src/*.cpp
        ./src/*.h
        ./src/*.ui
        )

foreach (cpp_h_ui_file_path ${cpp_h_ui_path})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" cpp_h_ui_filename ${cpp_h_ui_file_path})
    list(APPEND SOURCES_CPP_H_UI ${cpp_h_ui_filename})
endforeach (cpp_h_ui_file_path)

set(PROJECT_SOURCES
        ${SOURCES_CPP_H_UI}
        )

add_executable(${projectName}
        ${PROJECT_SOURCES}
        )

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Core
        Network
        )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
        Core
        Network
        )


target_link_libraries(${projectName}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt6::Network
        )

install(TARGETS ${projectName}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
