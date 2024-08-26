cmake_minimum_required(VERSION 3.29)
project("Speaky")

set(TEST_ENABLED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)

find_package(Boost REQUIRED CONFIG)
find_package(Protobuf REQUIRED CONFIG)

add_executable(${PROJECT_NAME} src/main.cc)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_BINARY_DIR} 
    ${Boost_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${Boost_LIBRARIES} 
    ${Protobuf_LIBRARIES}
)

if (TEST_ENABLED)
    enable_testing()
    add_subdirectory(tests)
endif()