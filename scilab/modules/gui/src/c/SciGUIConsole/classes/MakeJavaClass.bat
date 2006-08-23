@echo off
SET JAVAC="../../../../../../Java/jdk1.5.0_06/bin/javac.exe"
rem Compilation des classes 

%JAVAC% -classpath ..\..\..\..\..\..\Java\SWT\SWT.jar -d ..\..\..\..\..\..\Java\bin SciGUIConsole.java

@echo on