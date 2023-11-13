find_package(Catch2 2 REQUIRED)

add_executable(tests_vector tests_vector.cpp)

target_link_libraries(tests_vector vector Catch2::Catch2)