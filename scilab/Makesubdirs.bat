@echo off
REM Allan CORNET 2005 INRIA
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
if "%1" == "imp-clean" goto imp-clean
if "%1" == "imp-distclean" goto imp-distclean
if "%1" == "intersci" goto intersci
if "%1" == "intersci-clean" goto intersci-clean
if "%1" == "intersci-distclean" goto intersci-distclean
if "%1" == "dumpexts" goto dumpexts
if "%1" == "pvm" goto pvm
if "%1" == "def" goto def
if "%1" == "man" goto man
if "%1" == "man-clean" goto man-clean
if "%1" == "xmlint" goto xmlint
if "%1" == "xmlint-distclean" goto xmlint-distclean
if "%1" == "xmlint-clean" goto xmlint-clean
if "%1" == "check" goto check
if "%1" == "man-distclean" goto man-distclean
if "%1" == "tcl" goto tcl
if "%1" == "libs-distclean" goto libs-distclean
if "%1" == "libs-clean" goto libs-clean
if "%1" == "modelicac-distclean" goto modelicac-distclean
if "%1" == "modelicac-clean" goto modelicac-clean
if "%1" == "modelicac" goto modelicac
if "%1" == "dumpexts-clean" goto dumpexts-clean
if "%1" == "dumpexts-distclean" goto dumpexts-distclean
if "%1" == "scitests" goto scitests
if "%1" == "scitests-clean" goto scitests-clean
echo Unknown target %1 
goto end

:modelicac
cd ocaml
 nmake /C /f Makefile.mak all
cd ..
goto end

:modelicac-distclean
cd ocaml
 nmake /C /f Makefile.mak distclean
cd ..
goto end

:modelicac-clean
cd ocaml
 nmake /C /f Makefile.mak clean
cd ..
goto end

:dumpexts
cd Win-util\Nm
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
 nmake /C /f Makefile.mak all
cd ..
goto end

:imp-distclean
cd imp
 nmake /C /f Makefile.mak distclean
cd ..
goto end

:imp-clean
cd imp
 nmake /C /f Makefile.mak clean
cd ..
goto end

:intersci
cd intersci
 echo making all in intersci
 nmake /C /f Makefile.mak all 
cd ..
goto end

:intersci-clean
cd intersci
nmake /C /f Makefile.mak clean
cd ..
goto end

:intersci-distclean
cd intersci
nmake /C /f Makefile.mak distclean
cd ..
goto end

:xmlint 
cd Win-util\xmlint
nmake /C /f Makefile.mak all /a
cd ..\..
goto end

:check
if NOT EXIST man goto end
cd man
 echo checking all in man
 nmake /C /f makefile.mak check /a
cd ..
goto end

:man
if NOT EXIST man goto end
cd man
 echo making all in man
 nmake /C /f makefile.mak build /a
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
cd ..\.. 
goto end

:Javasci
cd routines\javasci
 echo making all Javasci
 nmake /C /f makefile.mak all /a
cd ..\.. 
goto end

:tcl
cd tcl
 echo making all in tcl
 nmake /C /f Makefile.mak 
cd ..
goto end

:man-clean
if NOT EXIST man goto end
cd man
 echo making clean in man
 nmake /C /f makefile.mak clean 
cd ..
goto end

:man-distclean
if NOT EXIST man goto end
cd man
 echo making distclean in man
 nmake /C /f makefile.mak distclean 
cd ..
goto end

:libs-distclean
cd libs
echo making distclean in libs
nmake /C /f Makefile.mak distclean
cd ..
goto end

:libs-clean
cd libs
echo making clean in libs
nmake /C /f Makefile.mak clean
cd ..
goto end

:xmlint-clean
cd Win-util\xmlint
echo making clean in xmlint
nmake /C /f Makefile.mak clean /a
cd ..\..
goto end

:xmlint-distclean
cd Win-util\xmlint
echo making clean in xmlint
nmake /C /f Makefile.mak distclean /a
cd ..\..
goto end

:dumpexts-distclean
cd Win-util\Nm
echo making clean in Nm
nmake /C /f Makefile.mak distclean /a
cd ..\..
goto end

:dumpexts-clean
cd Win-util\Nm
echo making clean in Nm
nmake /C /f Makefile.mak clean /a
cd ..\..
goto end

:scitests
if NOT EXIST tests goto end
cd tests\basic_tests
echo making Scilab tests
nmake /C /f Makefile.mak tests
cd ..
cd ..
goto end

:scitests-clean
if NOT EXIST tests goto end
cd tests\basic_tests
echo cleaning Scilab tests
nmake /C /f Makefile.mak tests-clean
cd ..
cd ..
goto end

:end 
