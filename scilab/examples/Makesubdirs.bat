@echo off
REM Makesubdirs.bat all
if "%1" == "all" goto all
if "%1" == "clean" goto clean 
if "%1" == "distclean" goto distclean 
if "%1" == "tests" goto tests


echo Unknown target %1 
goto end

:all 


rem echo "Inside interface-general"
rem cd interface-general 
rem  nmake /nologo /C /f Makefile.mak all
rem cd ..\

rem echo "Inside interface-tour-so"
rem cd interface-tour-so 
rem  nmake /nologo /C /f Makefile.mak all
rem cd ..\


echo "Inside interface-tutorial-so"
cd interface-tutorial-so
 nmake /nologo /C /f Makefile.mak all
cd ..\

echo "Inside interface-examples-so"
cd intersci-examples-so
 nmake /nologo /C /f Makefile.mak all
cd ..\

echo "Inside link-examples-so"
cd link-examples-so 
 nmake /nologo /C /f Makefile.mak all
cd ..\

rem echo "Inside mex-examples/cmex"
rem cd mex-examples/cmex
rem  nmake /nologo /C /f Makefile.mak all
rem cd ..\..\

rem echo "Inside mex-examples/cppmex"
rem cd mex-examples/cppmex
rem  nmake /nologo /C /f Makefile.mak all
rem cd ..\..\

goto end 

:clean 


echo "Inside interface-general"
cd interface-general 
 nmake /nologo /C /f Makefile.mak clean 
cd ..\


cd interface-tour-so 
 nmake /nologo /C /f Makefile.mak clean
cd ..\

cd interface-tutorial-so
 nmake /nologo /C /f Makefile.mak clean
cd ..\

cd intersci-examples-so
 nmake /nologo /C /f Makefile.mak clean
cd ..\

cd link-examples-so 
 nmake /nologo /C /f Makefile.mak clean
cd ..\

echo "Inside mex-examples/cmex"
cd mex-examples/cmex
 nmake /nologo /C /f Makefile.mak clean 
cd ..\..\

echo "Inside mex-examples/cppmex"
cd mex-examples/cppmex
 nmake /nologo /C /f Makefile.mak clean 
cd ..\..\
goto end 

:distclean

echo "Inside interface-general"
cd interface-general 
 nmake /nologo /C /f Makefile.mak distclean
cd ..\

cd interface-tour-so 
 nmake /nologo /C /f Makefile.mak distclean
cd ..\

cd interface-tutorial-so
 nmake /nologo /C /f Makefile.mak distclean
cd ..\

cd intersci-examples-so
 nmake /nologo /C /f Makefile.mak distclean
cd ..\

cd link-examples-so 
 nmake /nologo /C /f Makefile.mak distclean
cd ..\

echo "Inside mex-examples/cmex"
cd mex-examples/cmex
 nmake /nologo /C /f Makefile.mak distclean 
cd ..\..\

echo "Inside mex-examples/cppmex"
cd mex-examples/cppmex
 nmake /nologo /C /f Makefile.mak distclean 
cd ..\..\

goto end 

:tests

rem echo "Inside interface-general"
rem cd interface-general 
rem  nmake /nologo /C /f Makefile.mak tests
rem cd ..\

rem cd interface-tour-so 
rem  nmake /nologo /C /f Makefile.mak tests
rem cd ..\

cd interface-tutorial-so
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd intersci-examples-so
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd link-examples-so 
 nmake /nologo /C /f Makefile.mak tests
cd ..\

rem echo "Inside mex-examples/cmex"
rem cd mex-examples\cmex
rem  nmake /nologo /C /f Makefile.mak tests 
rem cd ..\..\

rem echo "Inside mex-examples/cppmex"
rem cd mex-examples\cppmex
rem  nmake /nologo /C /f Makefile.mak tests 
rem cd ..\..\

goto end 

:end 
