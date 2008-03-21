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

set flatXMLFile=%2\%~n1.xml

set cp=%libDir%\sci_doc_kit.jar;%libDir%\jeuclid-core.jar;%libDir%\commons-logging.jar;%libDir%\batik.jar;%libDir%\js.jar

java -cp "%cp%" org.scilab.doc_kit.CopyConvert "%1" "%flatXMLFile%"

java -Xmx512m -cp "%libDir%\saxon.jar;%libDir%\docbook-xsl-saxon.jar" com.icl.saxon.StyleSheet "%flatXMLFile%" "%docbookXslDir%\html\chunk.xsl" base.dir="%2\\" use.id.as.filename=1 html.stylesheet=html.css use.extensions=1 graphicsize.extension=0 toc.section.depth=3 section.autolabel=1

if exist "%flatXMLFile%" del /q "%flatXMLFile%"

copy "%cssDir%\html.css" "%2"

goto end
endlocal

:usage
echo Usage: sci2html in_xml_file out_html_directory
echo Converts an XML document conforming to scilab.rnc
echo to multi-page HTML.

:end
