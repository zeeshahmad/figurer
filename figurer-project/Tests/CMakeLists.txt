project(figurer-tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)


set(TEST_FILES
        statuswidget_test.cpp
        project_test.cpp
        projectfileio_test.cpp
        externalfilescanner_test.cpp
        projectmanager_test.cpp
        latexfileparser_test.cpp
)

foreach(testFile ${TEST_FILES})
	get_filename_component(testFilename ${testFile} NAME_WLE)
        qt_add_executable(${testFilename} MANUAL_FINALIZATION
            ${testFile}
            ../resources/resources.qrc)

        add_test(NAME ${testFilename} COMMAND ${testFilename})
        target_include_directories(${testFilename} PRIVATE
            "${PROJECT_SOURCE_DIR}/../include")
        target_link_libraries(${testFilename} PRIVATE
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Test
            figurer-lib)

        qt_finalize_executable(${testFilename})
endforeach()




