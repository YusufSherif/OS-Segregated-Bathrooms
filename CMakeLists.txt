cmake_minimum_required(VERSION 3.12)
project(Project_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_2
        main.c
        bathroom.h
        bathroom_queue.h
        person.h
        dispatch_utils.h)

target_link_libraries (Project_2 pthread)