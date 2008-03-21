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

rem MODIFY THIS IF NEEDED TO
set hhc=C:\Program Files\HTML Help Workshop\hhc.exe

set binDir=%~dp0
set libDir=%binDir%\..\..\..\thirdparty
set docbookXslDir=%binDir%\..\docbook_xsl
set cssDir=%binDir%\..\css

set tmpDir=%TEMP%\sci2chm_tmp
if exist "%tmpDir%" rmdir /s /q "%tmpDir%"
mkdir "%tmpDir%"

set flatXMLFile=%tmpDir%\%~n1.xml

copy "%cssDir%\htmlhelp.css" "%tmpDir%"

set cp=%libDir%\sci_doc_kit.jar;%libDir%\jeuclid-core.jar;%libDir%\commons-logging.jar;%libDir%\batik.jar;%libDir%\js.jar

java -cp "%cp%" org.scilab.doc_kit.CopyConvert "%1" "%flatXMLFile%"

java -Xmx512m -cp "%libDir%\saxon.jar;%libDir%\docbook-xsl-saxon.jar" com.icl.saxon.StyleSheet "%flatXMLFile%" "%docbookXslDir%\htmlhelp\htmlhelp.xsl"  base.dir="%tmpDir%\\" manifest.in.base.dir=1 html.stylesheet=htmlhelp.css use.extensions=1 graphicsize.extension=0 "generate.toc= "

if exist "%flatXMLFile%" del /q "%flatXMLFile%"

"%hhc%" "%tmpDir%\htmlhelp.hhp"
move /y "%tmpDir%\htmlhelp.chm" "%2"

if exist "%tmpDir%" rmdir /s /q "%tmpDir%"

goto end
endlocal

:usage
echo Usage: sci2chm in_xml_file out_chm_file
echo Converts an XML document conforming to scilab.rnc
echo to a Windows HTML Help ('.chm') file.
echo By default, this script assumes that hhc.exe is found
echo in "C:\Program Files\HTML Help Workshop\hhc.exe".
echo If this is not the case, please modify the 'hhc'
echo variable found at the beginning of this '.bat' file.

:end





