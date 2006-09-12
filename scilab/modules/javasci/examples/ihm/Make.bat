@echo off
rem Allan CORNET
rem INRIA Juillet 2004

rem A modifier selon votre répertoire d'installation de Java(TM) 2 SDK, Standard Edition Version 1.5
PATH=%PATH%;D:\TRUNK\scilab\modules\jvm\bin\jdk\bin;

rem Compilation des exemples
javac -deprecation -d ..\..\..\..\bin -classpath ..\..\..\..\bin *.java
echo on

