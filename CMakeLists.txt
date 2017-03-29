cmake_minimum_required(VERSION 3.6)
project(PPAP)

find_package(LLVM REQUIRED CONFIG)
find_package(PythonLibs REQUIRED)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_MODULE_PATH D:/msys64/mingw64/lib/cmake/llvm)
# include(LLVMConfig)

include_directories(
        ${PYTHON_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
        )
add_definitions(${LLVM_DEFINITIONS})

set(SOURCE_FILES src/parser.cpp src/convert.cpp src/ast.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)
set(LIBRARIES ${PYTHON_LIBRARIES} ${llvm_libs})
add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES})