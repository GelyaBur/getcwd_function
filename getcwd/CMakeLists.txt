cmake_minimum_required(VERSION 2.8)

set(EX "getcwd")

set(SOURCE "getcwd.c")

add_executable(${EX} ${SOURCE})

include_directories(lib)

add_subdirectory(lib)

target_link_libraries(${EX} lib)
