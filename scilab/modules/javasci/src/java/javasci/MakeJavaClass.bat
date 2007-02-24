@echo off
SET JAVAC="../../../../jvm/bin/jdk/bin/javac.exe"
SET JAVASCIPATH="..\..\..\..\..\bin"
rem Compilation des classes Java d'interfaces

%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% BadDataArgumentException.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% Scilab.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciDoubleArray.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciDouble.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciStringArray.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciString.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciComplexArray.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciComplex.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciBooleanArray.java
%JAVAC% -classpath %JAVASCIPATH% -d %JAVASCIPATH% SciBoolean.java
@echo on