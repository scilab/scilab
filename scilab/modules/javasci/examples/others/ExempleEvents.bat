echo off
SET JAVA="../modules/jvm/bin/jre/bin/java.exe"
rem execution des exemples
cd ..\..\..\..\bin
echo ------------------------------------------------------------------------
echo Exemple Events 1
echo ------------------------------------------------------------------------
%JAVA% ExempleEvent
pause
cd ..\examples\callsci\callsciJava\others
echo on
