@echo off
REM Makesubdirs.bat all
if "%1" == "all" goto all
if "%1" == "clean" goto clean 
if "%1" == "distclean" goto distclean 
if "%1" == "tests" goto tests


echo Unknown target %1 
goto end

:all 


echo "Inside interface-general"
cd interface-general 
 nmake /nologo /C /f Makefile.mak all
cd ..\

echo "Inside interface-tour-so"
cd interface-tour-so 
 nmake /nologo /C /f Makefile.mak all
cd ..\


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

echo "Inside mex-examples/cmex"
cd mex-examples/cmex
 nmake /nologo /C /f Makefile.mak all
cd ..\..\

echo "Inside mex-examples/cppmex"
cd mex-examples/cppmex
 nmake /nologo /C /f Makefile.mak all
cd ..\..\

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

echo "Inside interface-general"
cd interface-general 
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd interface-tour-so 
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd interface-tutorial-so
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd intersci-examples-so
 nmake /nologo /C /f Makefile.mak tests
cd ..\

cd link-examples-so 
 nmake /nologo /C /f Makefile.mak tests
cd ..\

echo "Inside mex-examples/cmex"
cd mex-examples\cmex
 nmake /nologo /C /f Makefile.mak tests 
cd ..\..\

echo "Inside mex-examples/cppmex"
cd mex-examples\cppmex
 nmake /nologo /C /f Makefile.mak tests 
cd ..\..\

goto end 

:end 
