@echo off
REM Makesubdirs.bat all
if "%1" == "routines" goto routines
if "%1" == "routines-clean" goto routines-clean 
if "%1" == "routines-distclean" goto routines-distclean 
if "%1" == "macros" goto macros
if "%1" == "macros-clean" goto macros-clean 
if "%1" == "macros-distclean" goto macros-distclean 
if "%1" == "wless" goto wless
if "%1" == "imp" goto imp
if "%1" == "intersci" goto intersci
if "%1" == "dumpexts" goto dumpexts
if "%1" == "pvm" goto pvm
if "%1" == "def" goto def

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

:wless
cd wless
 echo making all in wless
 nmake /C /f Makefile.mak 
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

:end 
