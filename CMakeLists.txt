cmake_minimum_required(VERSION 3.6)
project(PPAP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH /usr/local/Cellar/llvm/3.9.1/lib/cmake/llvm)
include(LLVMConfig)

include_directories(
        /usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/include/python3.6m
        ${LLVM_INCLUDE_DIRS}
        )
add_definitions(${LLVM_DEFINITIONS})

set(SOURCE_FILES src/parser.cpp src/convert.cpp src/ast.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)
set(LIBRARIES /usr/local/Cellar/python3/3.6.0_1/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6.dylib
        ${llvm_libs})

add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES} ${llvm_libs})