rem
rem $Id: make.bat,v 1.1 2001/04/26 07:47:04 scilab Exp $
rem
echo off
cls
rem Script File for PVM for WIN32
rem 06/24/1997  Markus Fischer
rem
:begin
if "%1" == "" goto intro
goto switch
:intro
echo This is a (small) script for (re)compiling the PVM code 
echo {and,or} the examples for the WIN32 environment. For
echo generic use, you must provide one of the following inputs:
echo.
echo make pvm        --   will recompile and reinstall the PVM code by compiling the
echo                      libpvm, daemon, hoster, groupserver and console
echo. 
echo make example c  --   will recompile the C examples
echo.
echo make example f  --   will recompile the Fortran examples
echo.
echo make all        --   will compile the items above
echo.
echo make clean      --   will erase the object files
echo.
echo make tidy       --   will erase the object and executables
echo.
echo Please note that the configuration file in $(PVM_ROOT)/conf/$(PVM_ARCH).def
echo should prompt to the installed Compiler and its INCLUDE and LIBRARY paths.
echo Bye.
goto Ende
:switch

if "%1" == "pvm" goto PVM
if "%1" == "all" goto PVMEX 
if "%1" == "clean" goto CLEAN
if "%1" == "tidy" goto TIDY
if "%1" == "example" goto excheck

echo Your specification did not match. Aborting.
echo Call make with no arguments for information.
goto Ende

:excheck
if "%2" == "c" goto CEXAMPLES
if "%2" == "f" goto FEXAMPLES
:PVM
SET PVM_ROOT=d:\softs\scilab-2.4\pvm3
echo Compiling and installing library (C,Fortran), daemon, tracer, console, hoster and groupserver
cd %PVM_ROOT%\src\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing tracer
cd %PVM_ROOT%\tracer\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing pvm
cd %PVM_ROOT%\console\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing hoster
cd %PVM_ROOT%\hoster\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing pvmgs
cd %PVM_ROOT%\pvmgs\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing libfpvm
cd %PVM_ROOT%\libfpvm\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Installation complete. Set your Environvariables !!!
echo More Information in Readme.win32
goto ENDE

:CEXAMPLES
cd %PVM_ROOT%\examples
echo Building C examples
nmake /f makefile.mak c-all
cd %PVM_ROOT%
goto Ende
:FEXAMPLES
echo Building Fortran examples
cd %PVM_ROOT%\examples
nmake /f makefile.mak f-all
cd %PVM_ROOT%
goto Ende

goto ENDE
:PVMEX
echo Compiling and installing library (C,Fortran), daemon, tracer, console, hoster and groupserver
cd %PVM_ROOT%\src\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing tracer
cd %PVM_ROOT%\tracer\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing pvm
cd %PVM_ROOT%\console\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing hoster
cd %PVM_ROOT%\hoster\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing pvmgs
cd %PVM_ROOT%\pvmgs\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing libfpvm
cd %PVM_ROOT%\libfpvm\
nmake /f makefile.mak
cd %PVM_ROOT%
echo Compiling and installing examples
cd %PVM_ROOT%\examples
echo Building C examples
nmake /f makefile.mak c-all
cd %PVM_ROOT%
echo Building Fortran examples
cd %PVM_ROOT%\examples
nmake /f makefile.mak f-all
cd %PVM_ROOT%
goto Ende
echo Installation complete. Set your Environment variables !!!
echo More Information in Readme.win32
goto ENDE
:TIDY
echo Cleaning Execs and Libs ...
cd %PVM_ROOT%\bin\%PVM_ARCH%\
erase *.exe
cd %PVM_ROOT%\console\%PVM_ARCH%\
erase *.exe
cd %PVM_ROOT%\tracer\%PVM_ARCH%\
erase *.exe
erase *.lib
cd %PVM_ROOT%\lib\win32\
erase *.lib
erase *.exe
cd %PVM_ROOT%\libfpvm\win32\
erase *.lib
cd %PVM_ROOT%
:CLEAN
echo Cleaning Objects ...
cd %PVM_ROOT%\src\%PVM_ARCH%\
erase *.obj
echo Cleaning libfpvm objects ...
cd %PVM_ROOT%\libfpvm\%PVM_ARCH%
erase *.obj
echo Cleaning console objects ...
cd %PVM_ROOT%\console\%PVM_ARCH%
erase *.obj
echo Cleaning hoster objects ...
cd %PVM_ROOT%\hoster\%PVM_ARCH%\
erase *.obj
echo Cleaning pvmgs objects ...
cd %PVM_ROOT%\pvmgs\%PVM_ARCH%\
erase *.obj
echo Cleaning tracer objects ...
cd %PVM_ROOT%\tracer\%PVM_ARCH%\
erase *.obj
echo Cleaning example objects ...
cd %PVM_ROOT%\examples\%PVM_ARCH%\
erase *.obj
cd %PVM_ROOT%
goto ENDE

:Ende
