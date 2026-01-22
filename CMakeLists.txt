cmake_minimum_required(VERSION 4.1)
project(TerminalUtility)

set(CMAKE_CXX_STANDARD 20)

add_executable(TerminalUtility src/main.cpp
        src/menu.h
        src/terminal.h
        src/menu.cpp
        src/input.h
        src/input.cpp
        src/form.h
)
