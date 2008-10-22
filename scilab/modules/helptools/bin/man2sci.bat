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

if "%2"=="" goto usage

set binDir=%~dp0
set libDir=%binDir%\..\..\..\thirdparty
set convDir=%binDir%\..\xsl\convert
set tmpFile=%1.manrev

java -jar "%libDir%\saxon.jar" -o "%tmpFile%" "%1" "%convDir%\man2manrev.xsl"

java -jar "%libDir%\saxon.jar" -o "%2" "%tmpFile%" "%convDir%\manrev2sci.xsl"

if exist "%tmpFile%" del /q "%tmpFile%"

goto end
endlocal

:usage
echo Usage: man2sci in_man_xml_file out_scilab_xml_file
echo Converts a document conforming to man.dtd to
echo a document conforming to scilab.rnc.

:end
