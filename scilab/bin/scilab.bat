@echo off
SETLOCAL ENABLEEXTENSIONS
SET PARENT=%~dp0
SET "START_CALL=start "Scilab" "
set BIN=WScilex.exe

:Argloop
SET CURRENT_ARG=%~1
rem echo arg %CURRENT_ARG%

IF "%CURRENT_ARG%"=="-nw" (
	SET BIN=WScilex-cli.exe
    SET START_CALL=
	goto next
)

IF "%CURRENT_ARG%"=="-nwni" (
	SET BIN=Scilex.exe
    SET START_CALL=
	goto next
)

IF "%CURRENT_ARG%"=="-nogui" (
	SET BIN=Scilex.exe
    SET START_CALL=
	goto next
)

IF "%CURRENT_ARG%"=="--help" (
    %PARENT%Scilex.exe --help
    goto quit
)

set ArgsExceptFirst=%ArgsExceptFirst% %1%

:next
shift

if not "%CURRENT_ARG%"=="" goto Argloop

rem echo.%START_CALL%"%PARENT%%BIN%"%ArgsExceptFirst%
%START_CALL%"%PARENT%%BIN%"%ArgsExceptFirst%

:quit
EXIT /B %ERRORLEVEL%
