@echo off
REM Makesubdirs.bat all

cd kernel
echo Macros compilation m2sci/kernel
nmake /C /f Makefile.mak %1 
cd ..

cd percent
echo Macros compilation m2sci/percent
nmake /C /f Makefile.mak %1 
cd ..

cd sci_files
echo Macros compilation m2sci/sci_files
nmake /C /f Makefile.mak %1 
cd ..

cd ..
