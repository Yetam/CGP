cmake_minimum_required(VERSION 3.5.1)
project(CmakeTest VERSION 1.0.0)

add_library(
	CGPlib
	CGPlib.h
	CGP.cpp
)

add_executable(CGP main.cpp)

target_link_libraries(CGP PRIVATE CGPlib)
