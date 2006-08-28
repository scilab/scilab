@echo off
SET JAVAC="../../../jvm/bin/jdk/bin/javac.exe"
rem Compilation des classes Java d'interfaces

%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin BadDataArgumentException.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin Scilab.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciDoubleArray.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciDouble.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciStringArray.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciString.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciComplexArray.java
%JAVAC% -classpath ..\..\..\..\bin -d ..\..\..\..\bin SciComplex.java

@echo on