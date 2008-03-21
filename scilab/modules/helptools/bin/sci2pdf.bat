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
rem This XSL style sheet fixes bugs in ..\docbook_xsl\fo\htmltbl.xsl
set docbookXslDir=%binDir%\..\xsl

set format=-ps
if "%~x2"==".ps" goto proceed
set format=-pdf
:proceed

set foDir=%TEMP%\sci2pdf_tmp
if exist "%foDir%" rmdir /s /q "%foDir%"
mkdir "%foDir%"

set flatXMLFile=%foDir%\%~n1.xml

set foFile=%foDir%\__doc.fo

set cp=%libDir%\sci_doc_kit.jar;%libDir%\jeuclid-core.jar;%libDir%\commons-logging.jar;%libDir%\batik.jar;%libDir%\js.jar

java -cp "%cp%" org.scilab.doc_kit.CopyConvert "%format%" "%1" "%flatXMLFile%"

java -Xmx512m -cp "%libDir%\saxon.jar;%libDir%\docbook-xsl-saxon.jar" com.icl.saxon.StyleSheet -o "%foFile%" "%flatXMLFile%" "%docbookXslDir%\fo\docbook.xsl" use.extensions=1 graphicsize.extension=0 paper.type=A4 "generate.toc=book toc,title,figure,table,example,equation part toc,title reference toc,title" toc.section.depth=3 section.autolabel=1 variablelist.as.blocks=1 shade.verbatim=1

set cp2=%libDir%\fop.jar;%libDir%\fop-hyph.jar;%libDir%\avalon-framework.jar;%libDir%\commons-io.jar;%libDir%\commons-logging.jar;%libDir%\xmlgraphics-commons.jar;%libDir%\jimi.jar;%libDir%\batik.jar;%libDir%\js.jar

java -Xmx512m -cp "%cp2%" org.apache.fop.cli.Main -dpi 120 -r -fo "%foFile%" "%format%" "%2"

if exist "%foDir%" rmdir /s /q "%foDir%"

goto end
endlocal

:usage
echo Usage: sci2pdf in_xml_file out_pdf_or_ps_file
echo Converts an XML document conforming to scilab.rnc to PDF
echo or to PostScript.
echo A PostScript file is generated if out_pdf_or_ps_file ends
echo with '.ps'.

:end



