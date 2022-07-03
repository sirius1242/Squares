cmake_minimum_required(VERSION 3.9)
project(squares CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

file(GLOB COMMON
	"include/board.hpp"
	"include/client.hpp"
	"include/server.hpp"
	"src/board.cpp"
)

add_executable(${PROJECT_NAME} ${COMMON} "src/client.cpp")
add_executable(${PROJECT_NAME}-sever ${COMMON} "src/server.cpp")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
