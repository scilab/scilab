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

java -jar "%libDir%\saxon.jar" -o "%2" "%1" "%convDir%\manrev2sci.xsl"

goto end
endlocal

:usage
echo Usage: manrev2sci in_manrev_xml_file out_scilab_xml_file
echo Converts a document conforming to manrev.dtd to
echo a document conforming to scilab.rnc.

:end
