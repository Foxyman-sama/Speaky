cmake_minimum_required(VERSION 3.29)
project(send_message_tests)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} send_message_tests.cc)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME}  
    GTest::gtest_main
    GTest::gmock_main
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
