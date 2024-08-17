cmake_minimum_required(VERSION 3.29)
project("Speaky")

set(TEST_ENABLED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 26)

find_package(Boost REQUIRED CONFIG)
find_package(Protobuf REQUIRED)

add_executable(${PROJECT_NAME} src/main.cxx)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_BINARY_DIR} 
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} 
    network_lib
    protobuf::libprotoc protobuf::libprotobuf
)

add_subdirectory(lib/network_lib)

protobuf_generate(TARGET ${PROJECT_NAME} PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/proto/chat.proto)
protobuf_generate(TARGET ${PROJECT_NAME} PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/proto/user_connection.proto)

if (TEST_ENABLED)
    enable_testing()
    add_subdirectory(tests)
endif()