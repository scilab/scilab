@echo off
REM Makesubdirs.bat all
if "%1" == "routines" goto routines
if "%1" == "routines-clean" goto routines-clean 
if "%1" == "routines-distclean" goto routines-distclean 
if "%1" == "macros" goto macros
if "%1" == "MakeMexLib" goto MakeMexLib
if "%1" == "Javasci" goto Javasci
if "%1" == "macros-clean" goto macros-clean 
if "%1" == "macros-distclean" goto macros-distclean 
if "%1" == "imp" goto imp
if "%1" == "intersci" goto intersci
if "%1" == "dumpexts" goto dumpexts
if "%1" == "pvm" goto pvm
if "%1" == "def" goto def
if "%1" == "man" goto man
if "%1" == "tcl" goto tcl
if "%1" == "man-clean" goto man-clean
if "%1" == "man-distclean" goto man-distclean


echo Unknown target %1 
goto end

:dumpexts
cd Win95-util\Nm
 nmake /C /f Makefile.mak 
cd ..\..
goto end 

:pvm 
cd pvm3
 nmake /C /f Makefile.mak 
cd ..
goto end 

:def 
cd libs
 nmake /C /f Makefile.mak 
cd ..
goto end 

:routines 
cd routines 
 nmake /C /f Makefile.mak 
cd ..
goto end 

:routines-distclean 
cd routines 
 nmake /C /f Makefile.mak distclean 
cd ..
goto end 

:routines-clean 
cd routines 
 nmake /C /f Makefile.mak clean 
cd ..
goto end 

:macros 
cd macros 
 nmake /C /f Makefile.mak 
cd ..
goto end

:macros-clean 
cd macros 
 nmake /C /f Makefile.mak clean
cd ..
goto end

:macros-distclean 
cd macros 
 nmake /C /f Makefile.mak distclean
cd ..
goto end

:imp
cd imp
 echo making all in imp
 nmake /C /f Makefile.mak 
cd ..
goto end

:intersci
cd intersci
 echo making all in intersci
 nmake /C /f Makefile.mak 
cd ..
goto end
 
:man
cd man
 echo making all in man
 nmake /C /f makehelp.mak /a
cd ..
goto end

:MakeMexLib
cd routines\default
 echo making all Mexlib
 nmake /C /f makemex.mak all /a
 @copy /Y mexlib.dll ..\..\bin\libmex.dll 
 @copy /Y mexlib.dll ..\..\bin\libmx.dll
 @copy /Y mexlib.dll ..\..\bin\libmat.dll
 @del /Q mexlib.def
 @del /Q mexlib.dll
 @del /Q mexlib.ilib
 @del /Q mexlib.exp
cd ../.. 
goto end

:Javasci
cd routines\javasci
 echo making all Javasci
 nmake /C /f makefile.mak all /a
cd ../.. 
goto end


:tcl
cd tcl
 echo making all in tcl
 nmake /C /f Makefile.mak 
cd ..
goto end

:man-clean
cd man
 echo making all in man
 nmake /C /f makehelp.mak clean 
cd ..
goto end

:man-distclean
cd man
 echo making all in man
 nmake /C /f makehelp.mak distclean 
cd ..
goto end

:end 
