echo off
rem A modifier selon votre répertoire d'installation de Java(TM) 2 SDK, Standard Edition Version 1.4.2
PATH=%PATH%;c:\Wintools\JBuilder9\jdk1.4\bin;

rem execution des exemples
cd ..\..\..\..\bin
echo ------------------------------------------------------------------------
echo Exemple 1
echo ------------------------------------------------------------------------
java Exemple1
pause
echo ------------------------------------------------------------------------
echo Exemple 2
echo ------------------------------------------------------------------------
java Exemple2
pause
echo ------------------------------------------------------------------------
echo Exemple 3
echo ------------------------------------------------------------------------
java Exemple3
pause
echo on
