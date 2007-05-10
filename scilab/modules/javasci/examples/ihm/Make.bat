@echo off
rem Allan CORNET
rem INRIA Mai 2007

set SCILAB_ROOT=../../../..
set PATH=%SCILAB_ROOT%/bin;%PATH%
set JAVAC="%SCILAB_ROOT%/java/jdk/bin/javac.exe"
if NOT EXIST %JAVAC% set %JAVAC%=javac.exe
set CLASSPATH=%SCILAB_ROOT%/java/jar/modules/javasci.jar;%CLASSPATH%

rem build examples

mkdir build
cd build
mkdir classes
cd ..

%JAVAC% -deprecation -d build/classes *.java
@echo on

