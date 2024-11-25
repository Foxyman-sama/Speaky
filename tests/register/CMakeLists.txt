cmake_minimum_required(VERSION 3.30)
project(register_tests)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED CONFIG)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} register_tests.cc)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}  
    GTest::gtest_main
    GTest::gmock_main
    ${Boost_LIBRARIES} 
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})