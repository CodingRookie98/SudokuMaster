cmake_minimum_required(VERSION 3.5)

project(SudokuMaster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)



set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    # Linux系统
    # 在这里添加针对Linux的特定配置
    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-Linux )
    Set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-Linux )
    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/minSizeRe-Linux )

elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    # Windows系统
    # 在这里添加针对Windows的特定配置
    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-Win )
    Set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-Win )
    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin/minSizeRe-Win )
endif ()

find_package ( QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Widgets
    LinguistTools
    Sql
)

find_package ( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    LinguistTools
    Sql
)

set ( TS_FILES SudokuMaster_zh_CN.ts )

set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install )

include_directories (
    ./3rdParty/include/
)

# 遍历所有.cpp、.h、.ui文件
file ( GLOB_RECURSE cpp_h_ui_path
    ./*.cpp
    ./*.h
    ./*.ui
)

foreach ( cpp_h_ui_file_path ${cpp_h_ui_path} )
    string ( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" cpp_h_ui_filename ${cpp_h_ui_file_path} )
    list ( APPEND SOURCES_CPP_H_UI ${cpp_h_ui_filename} )
endforeach ( cpp_h_ui_file_path )

set ( PROJECT_SOURCES

    ${SOURCES_CPP_H_UI}

    ${TS_FILES}
)

if ( ${QT_VERSION_MAJOR} GREATER_EQUAL 6 )
    qt_add_executable ( SudokuMaster

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

    # Define target properties for Android with Qt 6 as:

    # set_property(TARGET SudokuMaster APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR

    # ${CMAKE_CURRENT_SOURCE_DIR}/android)

    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation ( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

else ()
    if ( ANDROID )
        add_library ( SudokuMaster SHARED

            ${PROJECT_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:

    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable ( SudokuMaster

            ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation ( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )
endif ()

target_link_libraries ( SudokuMaster PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties ( SudokuMaster PROPERTIES

    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com

    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

install ( TARGETS SudokuMaster

    BUNDLE DESTINATION .

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )

if ( QT_VERSION_MAJOR EQUAL 6 )
    qt_finalize_executable ( SudokuMaster )
endif ()
