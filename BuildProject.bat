@echo off
echo Running this file will build the VS project of Vegetable Planting Information Management System
echo.
echo You need the CMake with a minimum version of 3.17
echo  and the Visual Stdio 2019 with C++ v14.20 MFC.
echo.
pause
mkdir build
cmake -S .\ -B .\build -G "Visual Studio 16 2019" -A "Win32"
mkdir .\build\src\Debug\bin
copy .\bin .\build\src\Debug\bin\
mkdir .\build\src\bin
copy .\bin .\build\src\bin\