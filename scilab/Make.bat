@echo off
if "%1" == "ABSOFT" set MAKE=%MAKE%
if "%1" == "VC" set MAKE=nmake /f Makefile.mak
if "%1" == "" goto ERROR_PARAM
if not "%PVM_ROOT%" == "" set COMPILE_PVM=yes
if not "%TKLIBS%" == "" goto L_TCL_INCLUDES
if not "%TCL_INCLUDES%" == "" goto L_TKLIBS
goto Make

:L_TCL_INCLUDES
if "%TCL_INCLUDES%" == "" goto TCL_INCLUDES_ERROR
set COMPILE_TCL_TK=yes
goto Make

:L_TKLIBS
if "%TKLIBS%" == "" goto TKLIBS_ERROR
set COMPILE_TCL_TK=yes

:Make
if "%COMPILE_PVM%" == "yes" goto MAKE_PVM
if "%COMPILE_TCL_TK%" == "yes" goto MAKE_TCL_TK
%MAKE%
goto end

:MAKE_PVM
if "%COMPILE_TCL_TK%" == "yes" goto MAKE_PVM_TCL_TK
%MAKE% PVM=libs/pvm.lib PVM_ROOT=%PVM_ROOT% PVM_ARCH=WIN32 PVMLIB="%PVM_ROOT%\lib\WIN32\libpvm3.lib %PVM_ROOT%\lib\WIN32\libgpvm3.lib" PVM_INCLUDES=-I%PVM_ROOT%\src DPVM=-DWITH_PVM
goto end

:MAKE_TCL_TK
if "%COMPILE_PVM%" == "yes" goto MAKE_PVM_TCL_TK
%MAKE% TKSCI=libs/tksci.lib TKLIBS=%TKLIBS% TKLIBSBIN=".\bin\tcl84.lib" ".\bin\tk84.lib" TCL_INCLUDES=%TCL_INCLUDES% DTK=-DWITH_TK
goto end

:MAKE_PVM_TCL_TK
%MAKE% PVM=libs/pvm.lib PVM_ROOT=%PVM_ROOT% PVM_ARCH=WIN32 PVMLIB=%PVM_ROOT%\lib\WIN32\libpvm3.lib %PVM_ROOT%\lib\WIN32\libgpvm3.lib PVM_INCLUDES=-I%PVM_ROOT%\src DPVM=-DWITH_PVM TKSCI=libs/tksci.lib TKLIBS=%TKLIBS% TKLIBSBIN=".\bin\tcl84.lib" ".\bin\tk84.lib" TCL_INCLUDES=%TCL_INCLUDES% DTK=-DWITH_TK
goto end

:TCL_INCLUDES_ERROR
@echo "ERROR: You must define the environment variable <<TCL_INCLUDES>>..."
goto end

:TKLIBS_ERROR
@echo "ERROR: You must define the environment variable <<TKLIBS>>..."
goto end

:ERROR_PARAM
@echo ERROR: You must choose one of these two options : "ABSOFT" or "VC"

:end
set COMPILE_PVM=
set COMPILE_TCL_TK=

