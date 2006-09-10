@echo off
rem Allan CORNET
rem INRIA Juillet 2004

rem A modifier selon votre répertoire d'installation de Java(TM) 2 SDK, Standard Edition Version 1.4.2
PATH=%PATH%;c:\Wintools\JBuilder9\jdk1.4\bin;

rem Compilation des exemples
javac -deprecation -d ..\..\..\..\bin -classpath ..\..\..\..\bin *.java
@echo on

