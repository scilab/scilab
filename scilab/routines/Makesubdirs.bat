@echo off
REM Makesubdirs.bat all
if "%1" == "all" goto all
if "%1" == "clean" goto all
if "%1" == "distclean" goto all
if "%1" == "tclsci" goto tclsci 
if "%1" == "pvm" goto pvm 

echo Unknown target %1 
goto end

:all
cd f2c\src 
echo Making %1 in directory  f2c\src 
 nmake /C /f Makefile.mak %1
cd ..\..
cd f2c\libf2c
echo Making %1 in directory  f2c\libf2c
 nmake /C /f Makefile.mak %1
cd ..\..
cd xdr
echo Making %1 in directory  xdr
 nmake /C /f Makefile.mak %1
cd ..
cd arpack
echo Making %1 in directory  arpack
 nmake /C /f Makefile.mak %1
cd ..

cd graphics
echo Making %1 in directory  graphics
 nmake /C /f Makefile.mak %1
cd ..
cd calelm
echo Making %1 in directory  calelm
 nmake /C /f Makefile.mak %1
cd ..
cd blas
echo Making %1 in directory  blas
 nmake /C /f Makefile.mak %1
cd ..
cd control
echo Making %1 in directory  control
 nmake /C /f Makefile.mak %1
cd ..
cd default
echo Making %1 in directory  default
 nmake /C /f Makefile.mak %1
cd ..
cd integ
echo Making %1 in directory  integ
 nmake /C /f Makefile.mak %1
cd ..
cd interf
echo Making %1 in directory  interf
 nmake /C /f Makefile.mak %1
cd ..
cd intersci
echo Making %1 in directory  intersci
 nmake /C /f Makefile.mak %1
cd ..
cd lapack
echo Making %1 in directory  lapack
 nmake /C /f Makefile.mak %1
cd ..
cd slicot
echo Making %1 in directory  scicot
 nmake /C /f Makefile.mak %1
cd ..
cd metanet
echo Making %1 in directory  metanet
 nmake /C /f Makefile.mak %1
cd ..
cd optim
echo Making %1 in directory  optim
 nmake /C /f Makefile.mak %1
cd ..
cd poly
echo Making %1 in directory  poly
 nmake /C /f Makefile.mak %1
cd ..
cd signal
echo Making %1 in directory  signal
 nmake /C /f Makefile.mak %1
cd ..
cd sparse
echo Making %1 in directory  sparse
 nmake /C /f Makefile.mak %1
cd ..
cd os_specific
echo Making %1 in directory  os_specific
 nmake /C /f Makefile.mak %1
cd ..
cd system
echo Making %1 in directory  system
 nmake /C /f Makefile.mak %1
cd ..
cd console
echo Making %1 in directory  console
 nmake /C /f Makefile.mak %1
cd ..
cd system2
echo Making %1 in directory  system2
 nmake /C /f Makefile.mak %1
cd ..
cd menusX
echo Making %1 in directory  menusX
 nmake /C /f Makefile.mak %1
cd ..
cd scicos
echo Making %1 in directory  scicos
 nmake /C /f Makefile.mak %1
cd ..
cd fileio
echo Making %1 in directory  fileio
 nmake /C /f Makefile.mak %1
cd ..
cd dcd 
echo Making %1 in directory  dcd 
 nmake /C /f Makefile.mak %1
cd ..
cd randlib
echo Making %1 in directory  randlib
 nmake /C /f Makefile.mak %1
cd ..
cd wintools
echo Making %1 in directory  wintools
 nmake /C /f Makefile.mak %1
cd ..
cd wsci
echo Making %1 in directory  wsci
 nmake /C /f Makefile.mak %1
cd ..
cd wsci\Wscilex
echo Making %1 in directory  wsci\WScilex
 nmake /C /f Makefile.mak %1
cd ..\..
cd wsci\CScilex
echo Making %1 in directory  wsci\CScilex
 nmake /C /f Makefile.mak %1
cd ..\..
cd gd
echo Making %1 in directory  gd
 nmake /C /f Makefile.mak %1
cd ..
cd int
echo Making %1 in directory  int
 nmake /C /f Makefile.mak %1
cd ..
REM cd if97
REM echo Making %1 in directory  if97
REM  nmake /C /f Makefile.mak %1
REM cd ..

cd tclsci
echo Making %1 in directory  tclsci
 nmake /C /f Makefile.mak %1
cd ..
echo on
goto end

:tclsci 
cd tclsci 
echo Making %1 in directory  tclsci 
 nmake /C /f Makefile.mak all
cd ..
goto end 

:pvm 
cd pvm 
echo Making %1 in directory  pvm 
 nmake /C /f Makefile.mak all
cd ..
goto end

:end 


