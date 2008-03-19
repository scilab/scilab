rem Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
rem Copyright (C) 2008 - INRIA
rem 
rem This file must be used under the terms of the CeCILL.
rem This source file is licensed as described in the file COPYING, which
rem you should have received as part of this distribution.  The terms
rem are also available at
rem http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

@echo off
setlocal

if "%1"=="" goto usage

set binDir=%~dp0
set libDir=%binDir%\..\..\..\thirdparty

java -cp "%libDir%\sci_doc_kit.jar;%libDir%\jhall.jar" org.scilab.doc_kit.HelpViewer %*

goto end
endlocal

:usage
echo Usage: sciviewhelp javahelp_jar_file ... javahelp_jar_file
echo Allows to browse the contents of one or more
echo JavaHelp[tm] .jar files created using sci2jh.
echo The name of a JavaHelp .jar file must end with '_help.jar'.

:end
