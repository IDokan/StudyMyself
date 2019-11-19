@echo off
rem `@echo off` tells windows to not print everyline of this batch file

rem From the path to this script file get the path to the root of the project
SET PROJECT_DIR_PATH=%~dp0

if DEFINED VS2019_ISREADY goto BuildProject

rem Setup Environment variables for VS2019
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
set VS2019_ISREADY="true"

:BuildProject

if "%1" == "" (
    call:echo_client
    call:echo_server
    goto:eof
)

if "%1" == "echo_client" (
    call:echo_client
    goto:eof
)

if "%1" == "echo_server" (
    call:echo_server
    goto:eof
)

if "%1" == "clean" (
    call:clean
    goto:eof
)

echo Unkown command given : "%1"
echo expected nothing, echo_client, echo_client, or clean
goto:eof

:echo_client
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -Feecho_client echo_client.cpp sockets.cpp Ws2_32.lib
    @echo off
goto:eof

:echo_server
    @echo on
    call cl /permissive- /GS /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /WX /Zc:forScope /Gd /Oy- /Oi /MD /std:c++17 /FC /EHsc /nologo /diagnostics:classic -Feecho_server echo_server.cpp sockets.cpp Ws2_32.lib
    @echo off
goto:eof

:clean
    @echo on
    call del /Q *.obj *.pdb echo_client.exe echo_server.exe
    @echo off
goto:eof