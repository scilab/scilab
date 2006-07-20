@echo off
rem A modifier selon votre répertoire d'installation de Java(TM) 2 SDK

PATH=%PATH%;C:\Wintools\JBuilder9\jdk1.4\bin;C:\j2sdk1.4.2_05\bin;C:\Wintools\j2sdk1.4.2_05\bin;C:\Wintools\Java\jdk1.5.0_01\bin;C:\Wintools\jdk1.5.0_01\bin

rem Compilation des classes Java d'interfaces

javac -classpath ..\..\..\bin -d ..\..\..\bin BadDataArgumentException.java
javac -classpath ..\..\..\bin -d ..\..\..\bin Scilab.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciDoubleArray.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciDouble.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciStringArray.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciString.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciComplexArray.java
javac -classpath ..\..\..\bin -d ..\..\..\bin SciComplex.java


@echo on