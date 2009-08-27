@echo off
rem Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
rem Copyright (C) 2007 - INRIA - Allan CORNET
rem
rem This file must be used under the terms of the CeCILL.
rem This source file is licensed as described in the file COPYING, which
rem you should have received as part of this distribution.  The terms
rem are also available at
rem http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

set SCILAB_ROOT=../../../..
set PATH=%SCILAB_ROOT%/bin;%PATH%
set JAVAC="%SCILAB_ROOT%/java/jdk/bin/javac.exe"
if NOT EXIST %JAVAC% set JAVAC=javac.exe
set CLASSPATH=%SCILAB_ROOT%/modules/javasci/jar/javasci.jar;%SCILAB_ROOT%/modules/javasci/jar/build/classes;%CLASSPATH%

rem build examples

mkdir build
cd build
mkdir classes
cd ..

%JAVAC% -deprecation -d build/classes *.java
@echo on

