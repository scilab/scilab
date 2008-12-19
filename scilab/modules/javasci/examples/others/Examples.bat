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
set JAVA="%SCILAB_ROOT%/java/jre/bin/java.exe"
if NOT EXIST %JAVA% set JAVA=java.exe
set CLASSPATH=%SCILAB_ROOT%/modules/javasci/jar/javasci.jar;%SCILAB_ROOT%/modules/javasci/examples/others/build/classes;%CLASSPATH%
echo ------------------------------------------------------------------------
echo Example 1
echo ------------------------------------------------------------------------
%JAVA% Example1
pause
echo ------------------------------------------------------------------------
echo Example 2
echo ------------------------------------------------------------------------
%JAVA% Example2
pause
echo ------------------------------------------------------------------------
echo Example 3
echo ------------------------------------------------------------------------
%JAVA% Example3
pause
echo ------------------------------------------------------------------------
echo Example 4
echo ------------------------------------------------------------------------
%JAVA% Example4
pause
echo ------------------------------------------------------------------------
echo Example 5
echo ------------------------------------------------------------------------
%JAVA% Example5
pause
echo ------------------------------------------------------------------------
echo Example 6
echo ------------------------------------------------------------------------
%JAVA% Example6
pause
echo ------------------------------------------------------------------------
echo Example 7
echo ------------------------------------------------------------------------
%JAVA% Example7A
%JAVA% Example7B
del Example7.ser
echo ------------------------------------------------------------------------
pause
echo Example 8
echo ------------------------------------------------------------------------
%JAVA% Example8
echo ------------------------------------------------------------------------
pause
echo Example 9
echo ------------------------------------------------------------------------
%JAVA% Example9
echo ------------------------------------------------------------------------
pause
echo Example 10
echo ------------------------------------------------------------------------
%JAVA% Example10
echo ------------------------------------------------------------------------
pause
echo Example 11
echo ------------------------------------------------------------------------
%JAVA% Example11
echo ------------------------------------------------------------------------
pause
echo Example 12
echo ------------------------------------------------------------------------
%JAVA% Example12
echo ------------------------------------------------------------------------
pause
echo Example Events
echo ------------------------------------------------------------------------
%JAVA% ExampleEvents
echo ------------------------------------------------------------------------
pause
echo Example Events 2
echo ------------------------------------------------------------------------
%JAVA% ExampleEvents2
echo ------------------------------------------------------------------------
pause
