@echo off 
rem -----------------------------------------------------
rem Allan CORNET
rem INRIA 2004
rem 9x,NT,XP
rem -----------------------------------------------------
IF "%OS%" == "Windows_NT" goto Continue
goto Windows9x
rem -----------------------------------------------------
:Continue
rem You can define your own Visual Studio Path
rem For example: my configuration VS .NET 2003
rem MSVCVARS32PATH=C:\Wintools\MICROS~1.NET\Common7\Tools\
rem MSVCVARS32=Vsvars32.bat

set MSVCVARS32PATH=""
set MSVCVARS32=""
rem -----------------------------------------------------
rem Visual .NET 2003 ? ? ?
if not "%VS71COMNTOOLS%" == "" goto VSNET2003
rem -----------------------------------------------------
rem Visual .NET 2002 ? ? ?
if not "%VSCOMNTOOLS%" == "" goto VSNET2002
rem -----------------------------------------------------
rem Visual Studio 6 ? ? ?
if not "%MSDevDir%" == ""  goto VS6
rem -----------------------------------------------------
goto ContinueNext
rem -----------------------------------------------------
:VSNET2002
set MSVCVARS32PATH=%VS70COMNTOOLS%
set MSVCVARS32=Vsvars32.bat
goto ContinueNext
rem -----------------------------------------------------
:VSNET2003
set MSVCVARS32PATH=%VS71COMNTOOLS%
set MSVCVARS32=Vsvars32.bat
goto ContinueNext
rem -----------------------------------------------------
:VS6
set MSVCVARS32=\bin\vcvars32.bat
if not "%MSVCDIR%" == ""  set MSVCVARS32PATH=%MSVCDIR%
goto ContinueNext
rem -----------------------------------------------------
:ContinueNext
if not "%MSVCVARS32PATH%"=="" call "%MSVCVARS32PATH%%MSVCVARS32%"
rem -----------------------------------------------------
:ChooseConsoleOrWindow
set PathScilab=%~d0%~p0
if "%1"=="" goto Wscilex
rem -----------------------------------------------------
:getparams
if "%1"=="" goto Wscilex
if "%1"=="-nw" goto Scilex
if "%1"=="-nwni" goto Scilex
shift
goto getparams
rem -----------------------------------------------------
:Windows9x
echo If you use MS VC++ 6, please be sure than Setting environment
echo for using Microsoft Visual C++ is OK.
echo See VCVARS32.BAT
start wscilex
goto end
rem -----------------------------------------------------
:Wscilex
start /d "%PathScilab%" wscilex.exe %*
goto end
rem -----------------------------------------------------
:Scilex
start /d "%PathScilab%" scilex.exe %*
rem -----------------------------------------------------
:end
@echo on
