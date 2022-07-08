cmake_minimum_required(VERSION 3.18)
project(ft_ssl C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

file(GLOB_RECURSE SOURCE_FILES "srcs/*/*.c" "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")

add_executable(ft_ssl ${SOURCE_FILES} ${HEADER_FILES})
include_directories(include)
