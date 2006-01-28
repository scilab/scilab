@echo off
rem A modifier selon votre répertoire d'installation de Java(TM) 2 SDK

PATH=%PATH%;C:\Wintools\JBuilder9\jdk1.4\bin;C:\j2sdk1.4.2_05\bin;C:\Wintools\j2sdk1.4.2_05\bin;C:\Wintools\Java\jdk1.5.0_01\bin;C:\Wintools\jdk1.5.0_01\bin

rem Compilation des classes Java d'interfaces

javac -cp ..\..\..\bin -d ..\..\..\bin BadDataArgumentException.java
javac -cp ..\..\..\bin -d ..\..\..\bin Scilab.java
javac -cp ..\..\..\bin -d ..\..\..\bin SciDoubleArray.java
javac -cp ..\..\..\bin -d ..\..\..\bin SciDouble.java
javac -cp ..\..\..\bin -d ..\..\..\bin SciStringArray.java
javac -cp ..\..\..\bin -d ..\..\..\bin SciString.java

@echo on