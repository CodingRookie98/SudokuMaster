set(targetName "SudokuMaster_client")
set(TS_FILES ./src/SudokuMaster_zh_CN.ts)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)


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
        ${TS_FILES}
        )

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Widgets
        LinguistTools
        Sql
        Network
        )

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
        Widgets
        LinguistTools
        Sql
        Network
        )

#if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${targetName}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ./resource/res.qrc
            )
    # Define target properties for Android with Qt 6 as:
    # set_property(TARGET SudokuMaster APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    # ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#else ()
#    if (ANDROID)
#        add_library(${targetName} SHARED
#                ${PROJECT_SOURCES}
#                )
#        # Define properties for Android with Qt 5 after find_package() calls as:
#        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else ()
#        add_executable(${targetName}
#                ${PROJECT_SOURCES}
#                )
#    endif ()
#    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#endif ()


find_package(spdlog REQUIRED PATHS ../3rdParty/lib)

target_link_libraries(${targetName} PRIVATE
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
        )
target_link_libraries(${targetName} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
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
