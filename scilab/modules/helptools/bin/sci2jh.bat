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
set docbookXslDir=%binDir%\..\docbook_xsl
set cssDir=%binDir%\..\css

set helpset=%~n2
set helpsetDir=%TEMP%\%helpset%

set flatXMLFile=%helpsetDir%\%~n1.xml

if exist "%helpsetDir%" rmdir /s /q "%helpsetDir%"
mkdir "%helpsetDir%"

copy "%cssDir%\javahelp.css" "%helpsetDir%"

set cp=%libDir%\sci_doc_kit.jar;%libDir%\jeuclid-core.jar;%libDir%\commons-logging.jar;%libDir%\batik.jar;%libDir%\js.jar

java -cp "%cp%" org.scilab.doc_kit.CopyConvert "%1" "%flatXMLFile%"

java -Xmx512m -cp "%libDir%\saxon.jar;%libDir%\docbook-xsl-saxon.jar" com.icl.saxon.StyleSheet "%flatXMLFile%" "%docbookXslDir%\javahelp\javahelp.xsl"  base.dir="%helpsetDir%\\" html.stylesheet=javahelp.css use.extensions=1 graphicsize.extension=0 "generate.toc= "

if exist "%flatXMLFile%" del /q "%flatXMLFile%"

rem Do it this way or full text search will not work.
set pwd=%CD%
chdir /d "%helpsetDir%" 
java -cp "%libDir%\jhall.jar" com.sun.java.help.search.Indexer .
chdir /d "%pwd%"

jar cf "%2" -C "%TEMP%" "%helpset%"

if exist "%helpsetDir%" rmdir /s /q "%helpsetDir%"

goto end
endlocal

:usage
echo Usage: sci2jh in_xml_file out_javahelp_jar_file
echo Converts an XML document conforming to scilab.rnc
echo to a JavaHelp[tm] .jar file.
echo out_javahelp_jar_file must end with '_help.jar'.

:end




