@echo off
rem `@echo off` tells windows to not print everyline of this batch file

if DEFINED VS2019_ISREADY goto BuildProject

rem Setup Environment variables for VS2019
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
set VS2019_ISREADY="true"

:BuildProject

if "%1" == "" (
    call:CppThread1
    call:CppThread2
    call:CppThreadMulti1
    call:CppThreadMulti2
    call:CppThreadData
    call:CppThreadMutex
    goto:eof
)

if "%1" == "CppThread1" (
    call:CppThread1
    goto:eof
)

if "%1" == "CppThread2" (
    call:CppThread2
    goto:eof
)

if "%1" == "CppThreadMulti1" (
    call:CppThread2
    goto:eof
)

if "%1" == "CppThreadMulti2" (
    call:CppThread2
    goto:eof
)

if "%1" == "CppThreadData" (
    call:CppThread2
    goto:eof
)

if "%1" == "CppThreadMutex" (
    call:CppThread2
    goto:eof
)

if "%1" == "clean" (
    call:clean
    goto:eof
)

echo Unkown command given : "%1"
echo expected nothing, CppThread1, CppThread1, CppThreadMulti1, CppThreadMulti2, CppThreadData, CppThreadMutex or clean
goto:eof

:CppThread1
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThread1 CppThread1.cpp
    @echo off
goto:eof

:CppThread2
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThread2 CppThread2.cpp
    @echo off
goto:eof

:CppThreadMulti1
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThreadMulti1 CppThreadMulti1.cpp
    @echo off
goto:eof

:CppThreadMulti2
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThreadMulti2 CppThreadMulti2.cpp
    @echo off
goto:eof

:CppThreadData
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThreadData CppThreadData.cpp
    @echo off
goto:eof

:CppThreadMutex
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeCppThreadMutex CppThreadMutex.cpp
    @echo off
goto:eof

:clean
    @echo on
    call del /Q *.obj *.pdb *.exe
    @echo off
goto:eof