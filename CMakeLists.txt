cmake_minimum_required(VERSION 3.6)
project(PPAP)

set(CMAKE_CXX_STANDARD 11)
include_directories(D:/Soft/Program/Python/Python36/include)
set(SOURCE_FILES
	src/parser.cpp
	src/convert.cpp
	src/ast.cpp
	)
set(LIBRARIES D:/Soft/Program/Python/Python36/libs/libpython36.a)
add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES})