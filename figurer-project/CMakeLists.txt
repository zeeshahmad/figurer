cmake_minimum_required(VERSION 3.5)

project(figurer-app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DISABLE_TESTS "Disable tests")

add_definitions(-DQT_NO_KEYWORDS)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Test)
find_package(Python COMPONENTS Interpreter Development)


file(COPY ${PROJECT_SOURCE_DIR}/texsoup DESTINATION python)

set(PROJECT_HEADERS
    include/app.h
    include/mainwindow.h
    include/codeeditor.h
    include/figureview.h
    include/statuswidget.h
    include/projecttools.h
    include/project.h
    include/projectfileio.h
    include/iexternalfileparser.h
    include/latexfileparser.h
    include/projectmanager.h
    include/externalfilescanner.h
    include/pycode.h
    include/pythonuser.h
)

set(PROJECT_SOURCES
    app.cpp
    mainwindow.cpp
    mainwindow.ui
    codeeditor.cpp
    figureview.cpp
    statuswidget.cpp
    projecttools.cpp
    project.cpp
    projectfileio.cpp
    iexternalfileparser.cpp
    latexfileparser.cpp
    projectmanager.cpp
    externalfilescanner.cpp
    pycode.cpp
)

add_subdirectory(QCodeEditor)
add_subdirectory(pybind11)

# FIGURER LIBRARY
qt_add_library(figurer-lib SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(figurer-lib PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(figurer-lib PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    QCodeEditor
    pybind11::embed)

#FIGURER APP
qt_add_executable(figurer-app
    MANUAL_FINALIZATION
    main.cpp
    resources/resources.qrc
)

target_include_directories(figurer-app
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(figurer-app PRIVATE
    figurer-lib)

set_target_properties(figurer-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.zeeshan.figurer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    CXX_VISIBILITY_PRESET hidden # for pybind warning e.g. when py::dict a field of pythonworker
)

install(TARGETS figurer-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
qt_finalize_executable(figurer-app)


if (DISABLE_TESTS)
    message(STATUS "Figurer testing disabled")
else()
    message(STATUS "Figurer testing enabled")
    file(COPY ${PROJECT_SOURCE_DIR}/texsoup DESTINATION Tests/python)
    enable_testing()
    add_subdirectory(Tests)
endif()
