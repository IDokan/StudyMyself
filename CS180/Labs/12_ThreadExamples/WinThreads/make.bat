@echo off
rem `@echo off` tells windows to not print everyline of this batch file

if DEFINED VS2019_ISREADY goto BuildProject

rem Setup Environment variables for VS2019
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
set VS2019_ISREADY="true"

:BuildProject

if "%1" == "" (
    call:WinThread1
    call:WinThread2
    call:WinThreadMulti1
    call:WinThreadMulti2
    call:WinThreadData
    call:WinThreadMutex
    goto:eof
)

if "%1" == "WinThread1" (
    call:WinThread1
    goto:eof
)

if "%1" == "WinThread2" (
    call:WinThread2
    goto:eof
)

if "%1" == "WinThreadMulti1" (
    call:WinThread2
    goto:eof
)

if "%1" == "WinThreadMulti2" (
    call:WinThread2
    goto:eof
)

if "%1" == "WinThreadData" (
    call:WinThread2
    goto:eof
)

if "%1" == "WinThreadMutex" (
    call:WinThread2
    goto:eof
)

if "%1" == "clean" (
    call:clean
    goto:eof
)

echo Unkown command given : "%1"
echo expected nothing, WinThread1, WinThread1, WinThreadMulti1, WinThreadMulti2, WinThreadData, WinThreadMutex or clean
goto:eof

:WinThread1
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThread1 WinThread1.cpp
    @echo off
goto:eof

:WinThread2
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThread2 WinThread2.cpp
    @echo off
goto:eof

:WinThreadMulti1
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThreadMulti1 WinThreadMulti1.cpp
    @echo off
goto:eof

:WinThreadMulti2
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThreadMulti2 WinThreadMulti2.cpp
    @echo off
goto:eof

:WinThreadData
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThreadData WinThreadData.cpp
    @echo off
goto:eof

:WinThreadMutex
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Od /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -FeWinThreadMutex WinThreadMutex.cpp
    @echo off
goto:eof

:clean
    @echo on
    call del /Q *.obj *.pdb *.exe
    @echo off
goto:eof