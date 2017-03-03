cmake_minimum_required(VERSION 3.6)
project(PPAP)

set(CMAKE_CXX_STANDARD 11)
# add_compile_options(-v)
# add_compile_options(-D Py_BUILD_CORE)

# file(GLOB_RECURSE include_files cpython/Include/*.h)
include_directories(
        /usr/local/Cellar/python3/3.6.0/Frameworks/Python.framework/Versions/3.6/include/python3.6m
)
# link_directories(
#         /usr/local/Cellar/python3/3.6.0/Frameworks/Python.framework/Versions/3.6/lib
# )

# file(GLOB_RECURSE src_files cpython/Python/*.c src/parser.cpp)
set(SOURCE_FILES src/parser.cpp)
set(LIBRARIES /usr/local/Cellar/python3/3.6.0/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6.dylib)
# add_executable(PPAP ${SOURCE_FILES})
add_executable(PPAP ${SOURCE_FILES})
target_link_libraries(PPAP ${LIBRARIES})
