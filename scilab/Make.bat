@cls
@echo off
rem /**********************************************************/
rem Allan CORNET INRIA (December 2004)
rem /**********************************************************/
set MAKE=nmake /f Makefile.mak
set PARAMS_COMPILE=
if "%1"=="" goto Message
if "%1" == "minimum" goto min
if "%1" == "standard" goto std
if "%1" == "all" goto all
if "%1" == "mexlib" goto mexlib
if "%1" == "javasci" goto javasci
if "%1" == "man" goto man
if "%1" == "macros" goto macros
if "%1" == "clean" goto clean
if "%1" == "distclean" goto distclean
if "%1" == "modelicac" goto modelicac
if "%1" == "xmlint" goto xmlint
if "%1" == "check" goto check
if "%1" == "tests" goto tests
if "%1" == "tests-clean" goto tests-clean
goto all
rem /**********************************************************/
:min
set PARAMS_COMPILE=minimum DLPVM=NO	DPVM=	DTK=
goto Make
rem /**********************************************************/
:std
set PARAMS_COMPILE=standard DLPVM=NO DPVM= DTK=-DWITH_TK
goto Make
rem /**********************************************************/
:all
set PARAMS_COMPILE=all DLPVM=YES DPVM=-DWITH_PVM DTK=-DWITH_TK 
goto Make
rem /**********************************************************/
:mexlib
set PARAMS_COMPILE=MakeMexLib
goto Make
rem /**********************************************************/
:javasci
set PARAMS_COMPILE=Javasci
goto Make
rem /**********************************************************/
:man
set PARAMS_COMPILE=man
goto Make
rem /**********************************************************/
:macros
set PARAMS_COMPILE=macros
goto Make
rem /**********************************************************/
:modelicac
set PARAMS_COMPILE=modelicac
goto Make
rem /**********************************************************/
:check
set PARAMS_COMPILE=check
goto Make
rem /**********************************************************/
:tests
set PARAMS_COMPILE=scitests
goto Make
rem /**********************************************************/
:xmlint
set PARAMS_COMPILE=xmlint
goto Make
rem /**********************************************************/
:tests-clean
set PARAMS_COMPILE=scitests-clean
goto Make
rem /**********************************************************/
:clean
set PARAMS_COMPILE=clean
del bin\*.exe
del bin\*.exp
del bin\*.ilk
del bin\*.pdb
del bin\javasci.dll
del bin\javasci.lib
del bin\libmat.dll
del bin\libmex.dll
del bin\libmx.dll
cd pvm3
del *.obj /s
del *.lib /s
del *.exe /s
cd ..
goto Make
rem /**********************************************************/
:distclean
set PARAMS_COMPILE=distclean
del bin\*.exp
del bin\*.ilk
del bin\*.pdb
cd pvm3
del *.obj /s
cd ..
goto Make
rem /**********************************************************/
:Make
%MAKE% %PARAMS_COMPILE%
goto end
rem /**********************************************************/
:Message
echo ***********************************************************************
echo *                                Scilab                               * 
echo ***********************************************************************
echo "%0 all" build Scilab (pvm,tcl/tk,macros,man,javasci,mexlib,modelicac,...)
echo  1] edit scilab/makefile.incl.mak
echo     modify (Line 60) PVM_ROOT=D:\scilab\pvm3 with a correct path
echo  2] you need ocaml
echo     edit scilab/ocaml/makefile.mak
echo     change OCAMLPATH=C:\Program Files\Objective Caml with a correct path 
echo  3] you need tcl/tk 8.4 or greater (see Readme_windows.txt)
echo  4] you need Java SDK 1.4.2 or greater
echo     Java must be in Path environment variable
echo "%0 standard" build Scilab (no pvm,tcl/tk,macros)
echo "%0 minimum" build Scilab (no pvm,no tcl/tk,macros)
echo "%0 mexlib" build mexlib interfaces
echo "%0 javasci" build Java Scilab interface
echo     you need Java SDK 1.4.2 or greater 
echo     Java must be in Path environment variable
echo "%0 check" check manuals
echo "%0 man" build manuals
echo "%0 macros" build macros
echo "%0 modelicac" build modelicac
echo     you need ocaml
echo     edit scilab/ocaml/makefile.mak
echo     change OCAMLPATH=C:\Program Files\Objective Caml with a correct path 
echo "%0 clean" clean Scilab for rebuild
echo "%0 distclean" clean Scilab for distribution
echo "%0 tests-clean" clean Scilab tests and prepare for %0 tests
echo "%0 tests" perform Scilab tests (execute %0 tests-clean before)
rem /**********************************************************/
:end
@echo on

