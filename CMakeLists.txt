#CMake Version
cmake_minimum_required(VERSION 3.17)
#Project Name
project(VegInfoSystem)
#Use the dynamic MFC library
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_AFXDLL)
#Unicode
add_definitions(-DUNICODE -D_UNICODE)
#Source directories
add_subdirectory(src)