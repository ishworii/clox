cmake_minimum_required(VERSION 4.0)
project(clox C)

set(CMAKE_C_STANDARD 11)

add_executable(clox main.c
        chunk.c
        memory.c
        debug.c
        value.c
        vm.c)
