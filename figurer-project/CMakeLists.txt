cmake_minimum_required(VERSION 3.5)

project(figurer-project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

#https://stackoverflow.com/questions/11041299/python-h-no-such-file-or-directory
#https://cmake.org/cmake/help/latest/module/FindPython3.html
find_package (Python COMPONENTS Interpreter Development)

file(COPY ${PROJECT_SOURCE_DIR}/texsoup DESTINATION python)


include(CMakePrintHelpers)
cmake_print_variables(PROJECT_SOURCE_DIR)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        pythonhighlighter.cpp
        pythonhighlighter.h
        codeeditor.cpp
        codeeditor.h
)

add_subdirectory(QCodeEditor)
add_subdirectory(json)

#add_subdirectory(boost)

#https://stackoverflow.com/questions/46571438/turning-the-no-keyword-qt-option-on-in-a-cmake-file-to-work-with-boostsignal
add_definitions(-DQT_NO_KEYWORDS)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(figurer-project
        MANUAL_FINALIZATION
        resources/resources.qrc
        ${PROJECT_SOURCES}
    )
    target_include_directories(figurer-project
        PRIVATE ${PROJECT_SOURCE_DIR}
        PUBLIC ${Python_INCLUDE_DIRS}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET figurer-project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(figurer-project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(figurer-project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(figurer-project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    QCodeEditor
    nlohmann_json::nlohmann_json
    #Boost::process
    #wsock32
    ${Python_LIBRARIES})

set_target_properties(figurer-project PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS figurer-project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(figurer-project)
endif()
