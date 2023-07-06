set(targetName "SudokuMaster_client")
set(TS_FILES ./src/SudokuMaster_zh_CN.ts)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux系统
    # 在这里添加针对Linux的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/Debug)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux/minSizeRe)
    # 设置默认的构建目录
    if (NOT DEFINED CMAKE_BINARY_DIR)
        set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build-linux)
    endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows系统
    # 在这里添加针对Windows的特定配置
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/Debug)
    Set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/Win/minSizeRe)
    # 设置默认的构建目录
    if (NOT DEFINED CMAKE_BINARY_DIR)
        set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build-win)
    endif ()
endif ()
# 遍历所有.cpp、.h、.ui文件
file(GLOB_RECURSE cpp_h_ui_path
        ./src/*.cpp
        ./src/*.h
        ./src/*.ui
        ../publicFile/*.h
        ../publicFile/*.cpp
        )

foreach (cpp_h_ui_file_path ${cpp_h_ui_path})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" cpp_h_ui_filename ${cpp_h_ui_file_path})
    list(APPEND SOURCES_CPP_H_UI ${cpp_h_ui_filename})
endforeach (cpp_h_ui_file_path)

set(PROJECT_SOURCES
        ${SOURCES_CPP_H_UI}
        ${TS_FILES}
        )

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Widgets
        LinguistTools
        Network
        )

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
        Widgets
        LinguistTools
        Network
        )

find_package(OpenSSL REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${targetName}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ./resource/res.qrc
            )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    add_executable(${targetName}
            ${PROJECT_SOURCES}
            )
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()


target_link_libraries(${targetName} PRIVATE spdlog::spdlog)
target_link_libraries(${targetName} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${targetName} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        )

set_target_properties(${targetName} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS ${targetName}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${targetName})
endif ()
