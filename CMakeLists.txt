cmake_minimum_required(VERSION 3.21)
project(cocktail C)

set(CMAKE_C_STANDARD 99)

add_library(cocktail SHARED server.c routing.c handler.c routing.h)
