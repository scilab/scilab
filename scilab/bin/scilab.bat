@echo off 

rem Allan CORNET
rem INRIA 2004
rem for XP and 2K

set PathScilab=%~d0%~p0
if "%1"=="" goto Wscilex

:getparams
if "%1"=="" goto Wscilex
if "%1"=="-nw" goto Scilex
if "%1"=="-nwni" goto Scilex
shift
goto getparams

:Wscilex
start %PathScilab%wscilex %*
goto end

:Scilex
%PathScilab%scilex %*

:end
@echo on