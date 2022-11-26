cmake_minimum_required(VERSION 3.23)
project(create_a_motion)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(.)

add_executable(create_a_motion
        json.cpp
        json.h
        json_reader.cpp
        json_reader.h
        main.cpp movable_object.h render.h render.cpp static_objects.h)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})