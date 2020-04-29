cmake_minimum_required(VERSION 3.15)
project(Project_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_2
        main.c
        bathroom.h
        bathroom_queue.h
        person.h
        dispatch_utils.h)