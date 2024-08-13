cmake_minimum_required(VERSION 3.29)
project("Speaky")

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)

find_package(Boost REQUIRED CONFIG)

add_library(${PROJECT_NAME} src/main.cxx)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} network_lib)

enable_testing()

add_subdirectory(lib/network_lib)