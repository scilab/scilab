@echo off

@echo off
rem  Only to check that examples can be built
cd ..\..\core\examples\call_scilab\cpp\common
nmake /nologo /f makefile.mak all >NUL
cd ..\..\..\..\..\development_tools\examples

rem  Only to check that examples can be built
cd ..\..\core\examples\call_scilab\c\common
nmake /nologo /f makefile.mak all >NUL
cd ..\..\..\..\..\development_tools\examples

cd ..\..\differential_equations\examples
nmake /nologo /f makefile.mak tests
cd ..\..\development_tools\examples

cd ..\..\incremental_link\examples\dynamic_links_c_and_fortran
nmake /nologo /f makefile.mak tests
cd ..\..\..\development_tools\examples

cd ..\..\incremental_link\examples\gateway
nmake /nologo /f makefile.mak tests
cd ..\..\..\development_tools\examples

cd ..\..\incremental_link\examples\interfaces\c
nmake /nologo /f makefile.mak all /A > NUL
nmake /nologo /f makefile.mak tests /A
cd ..\..\..\..\development_tools\examples

cd ..\..\incremental_link\examples\interfaces\fortran
nmake /nologo /f makefile.mak all /A > NUL
nmake /nologo /f makefile.mak tests /A
cd ..\..\..\..\development_tools\examples

cd ..\..\linear_algebra\examples
nmake /nologo /f makefile.mak all /A >NUL
nmake /nologo /f makefile.mak tests /A
cd ..\..\development_tools\examples

rem problems with Intersci :( 
cd ..\..\intersci\examples
nmake /nologo /f makefile.mak tests
cd ..\..\development_tools\examples

