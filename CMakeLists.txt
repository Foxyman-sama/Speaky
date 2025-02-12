cmake_minimum_required(VERSION 3.30)
project(Speaky)

set(TEST_ENABLED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)

find_package(Boost REQUIRED CONFIG)

add_executable(${PROJECT_NAME} src/main.cc)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${Boost_LIBRARIES} 
)

add_subdirectory(Packages)

if (TEST_ENABLED)
    enable_testing()
    add_subdirectory(tests)
endif()
