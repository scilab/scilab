@echo off
SET JAVAC="..\..\..\..\..\java\jdk\bin\javac.exe"
SET JAVASCIPATH="..\..\..\..\..\bin"
SET SRCCLASSPATH=".."
rem Compilation of the Scilab/Java interface

%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% BadDataArgumentException.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% Scilab.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciAbstractArray.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciDoubleArray.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciDouble.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciStringArray.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciString.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciComplexArray.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciComplex.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciBooleanArray.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% SciBoolean.java
%JAVAC% -classpath %SRCCLASSPATH% -d %JAVASCIPATH% ClassPath.java
@echo on