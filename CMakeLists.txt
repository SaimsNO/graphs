cmake_minimum_required(VERSION 3.0.0)
project(graphic VERSION 0.1.0 LANGUAGES C CXX)


set (SOURCES
    main.cpp
    mainwindow.cpp
    qcustomplot.cpp
)

set (HEADERS
    mainwindow.h
    qcustomplot.h
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/generated_files")

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Qt5_DIR "D:/programms/Qt/5.15.2/msvc2019_64")

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/binr)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Concurrent REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Qt5Widgets_LIBRARIES} Qt5::PrintSupport Qt5::Concurrent)

if(WIN32)
    # Путь к windeployqt (корректируйте в зависимости от вашей установки Qt)
    set(WINDEPLOYQT_EXECUTABLE "${Qt5_DIR}/bin/windeployqt.exe")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-compiler-runtime --no-angle "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMENT "Running windeployqt to copy Qt DLLs"
    )
endif()
