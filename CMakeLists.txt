cmake_minimum_required(VERSION 3.6)
project(PPAP)

set(CMAKE_CXX_STANDARD 11)
# add_compile_options(-v)

include_directories(
        /usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/include/python3.6m
        /usr/local/Cellar/llvm/3.9.1/include
        )

set(SOURCE_FILES src/parser.cpp src/convert.cpp src/ast.cpp)
set(LIBRARIES /usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6.dylib
        /usr/local/Cellar/llvm/3.9.1/lib/libLTO.dylib
        /usr/local/Cellar/llvm/3.9.1/lib/libLLVM.dylib
        )

add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES})
