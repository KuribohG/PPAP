cmake_minimum_required(VERSION 3.6)
project(PPAP)

find_package(LLVM REQUIRED CONFIG)
find_package(PythonLibs REQUIRED)

set(CMAKE_CXX_STANDARD 14)
<<<<<<< HEAD
# set(CMAKE_MODULE_PATH D:/msys64/mingw64/lib/cmake/llvm)
# include(LLVMConfig)

include_directories(
        ${PYTHON_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
        )
=======

set(PYTHON_INCLUDE_DIRS /usr/local/Frameworks/Python.framework/Versions/3.6/include/python3.6m)
set(PYTHON_LIBRARIES /usr/local/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6m.dylib)

set(CMAKE_MODULE_PATH /usr/local/Cellar/llvm/4.0.0/lib/cmake/llvm)
include(LLVMConfig)

# find_package(PythonLibs 3.6 REQUIRED)
# find_package(LLVM REQUIRED CONFIG)

include_directories(${PYTHON_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
>>>>>>> origin/master
add_definitions(${LLVM_DEFINITIONS})

set(SOURCE_FILES src/parser.cpp src/convert.cpp src/ast.cpp)

<<<<<<< HEAD
llvm_map_components_to_libnames(llvm_libs support core irreader)
set(LIBRARIES ${PYTHON_LIBRARIES} ${llvm_libs})
=======
llvm_map_components_to_libnames(LLVM_LIBRARIES support core irreader)
set(LIBRARIES ${PYTHON_LIBRARIES} ${LLVM_LIBRARIES})

>>>>>>> origin/master
add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES})