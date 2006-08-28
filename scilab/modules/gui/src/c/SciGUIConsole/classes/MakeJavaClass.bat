@echo off
SET JAVAC="../../../../../jvm/bin/jdk/bin/javac.exe"
rem Compilation des classes 

%JAVAC% -classpath ../../../../../jvm/bin/SWT/SWT.jar -d ../../../../../jvm/classes SciGUIConsole.java

@echo on