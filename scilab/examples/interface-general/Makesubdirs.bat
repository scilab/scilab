@echo off
REM Makesubdirs.bat all
if "%1" == "lib-clean" goto lib-clean 
if "%1" == "lib-distclean" goto lib-distclean 
if "%1" == "lib" goto lib
if "%1" == "def" goto def

echo Unknown target %1 
goto end

:def 
 nmake /nologo /C /f Makefile.mak 
goto end 

:lib 
cd lib 
 nmake /nologo /C /f Makefile.mak 
cd ..
goto end 

:lib-distclean 
cd lib 
 nmake /nologo /C /f Makefile.mak distclean 
cd ..
goto end 

:lib-clean 
cd lib
 nmake /nologo /C /f Makefile.mak clean 
cd ..
goto end 

:end 
