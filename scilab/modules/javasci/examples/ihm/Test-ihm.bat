@echo off
rem Allan CORNET
rem INRIA 2006

set SCILAB_ROOT=../../../..
set PATH=%SCILAB_ROOT%/bin;%PATH%
set JAVA="%SCILAB_ROOT%/java/jre/bin/java.exe"
if NOT EXIST %JAVA% set %JAVA%=java.exe
set CLASSPATH=%SCILAB_ROOT%/java/jar/modules/javasci.jar;build/classes;%CLASSPATH%

%JAVA% Test
echo on

