cmake_minimum_required(VERSION 3.20)
project(myTemplate)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vector)
add_subdirectory(history)
set_target_properties(vector PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} vector history)