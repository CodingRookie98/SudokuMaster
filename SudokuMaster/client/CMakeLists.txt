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

#set(spdlogDIR "C:/devTools/msys64/mingw64/lib/cmake/spdlog")
#set(fmtDIR "C:/devTools/msys64/mingw64/lib/cmake/fmt")
#find_package(spdlog CONFIG REQUIRED)
#link_directories("C:/devTools/msys64/mingw64/lib")

set ( TS_FILES ./src/SudokuMaster_zh_CN.ts )

set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install )

# 遍历所有.cpp、.h、.ui文件
file ( GLOB_RECURSE cpp_h_ui_path
    ./src/*.cpp
    ./src/*.h
    ./src/*.ui
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
    qt_add_executable ( SudokuMaster_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Define target properties for Android with Qt 6 as:
    # set_property(TARGET SudokuMaster APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation ( QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )

endif ()

target_link_libraries ( SudokuMaster_client PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
#        spdlog::spdlog
)

set_target_properties ( SudokuMaster_client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install ( TARGETS SudokuMaster_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if ( QT_VERSION_MAJOR EQUAL 6 )
    qt_finalize_executable ( SudokuMaster_client )
endif ()
