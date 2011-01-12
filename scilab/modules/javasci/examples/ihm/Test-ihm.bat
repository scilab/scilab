@echo off
rem Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
rem Copyright (C) 2006 - INRIA - Allan CORNET
rem
rem This file must be used under the terms of the CeCILL.
rem This source file is licensed as described in the file COPYING, which
rem you should have received as part of this distribution.  The terms
rem are also available at
rem http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
dem Note that this documentation is related to Javasci v1. Please use Javasci v2.




set SCILAB_ROOT=../../../..
set PATH=%SCILAB_ROOT%/bin;%PATH%
set JAVA="%SCILAB_ROOT%/java/jre/bin/java.exe"
if NOT EXIST %JAVA% set %JAVA%=java.exe
set CLASSPATH=%SCILAB_ROOT%/modules/javasci/jar/javasci.jar;%SCILAB_ROOT%/modules/javasci/examples/ihm/build/classes;%CLASSPATH%

%JAVA% Test
echo on

