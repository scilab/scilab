
SCIDIR=.
SCIDIR1=.

include Makefile.incl.mak 

minimum :: routines-pvm bin\scilex.exe macros bin\Wscilex.exe imp intersci
standard:: routines-pvm bin\scilex.exe macros bin\Wscilex.exe imp intersci 
all ::  modelicac routines-pvm bin\scilex.exe macros bin\Wscilex.exe MakeMexLib imp intersci Javasci man
allVC7 ::  modelicac routines-pvm bin\scilex.exe macros bin\Wscilex.exe MakeMexLib imp intersci Javasci xmlint check man

!IF "$(DLPVM)" == "YES"
routines-pvm :: routines pvm 
!ELSE 
routines-pvm :: routines 
!ENDIF

modelicac:
  Makesubdirs.bat modelicac

pvm	:
	Makesubdirs.bat pvm

routines:: 
	Makesubdirs.bat dumpexts
	Makesubdirs.bat routines
	Makesubdirs.bat def

macros::
	Makesubdirs.bat macros

imp::
	Makesubdirs.bat imp

intersci::
	Makesubdirs.bat intersci

xmlint::
	Makesubdirs.bat xmlint

check::
	Makesubdirs.bat check

man::
	Makesubdirs.bat man 

tcl:: 
	Makesubdirs.bat tcl 

MakeMexLib::
	Makesubdirs.bat MakeMexLib

Javasci:
	Makesubdirs.bat Javasci

scitests:
	Makesubdirs.bat scitests

scitests-clean:
	Makesubdirs.bat scitests-clean

clean::
	Makesubdirs.bat routines-clean
	Makesubdirs.bat macros-clean
	Makesubdirs.bat libs-clean
	Makesubdirs.bat man-clean
	Makesubdirs.bat modelicac-clean
	Makesubdirs.bat intersci-clean
	Makesubdirs.bat imp-clean
	Makesubdirs.bat xmlint-clean
	Makesubdirs.bat dumpexts-clean
  
distclean::
	Makesubdirs.bat routines-distclean 
	Makesubdirs.bat macros-distclean 
	Makesubdirs.bat man-distclean 
	Makesubdirs.bat libs-distclean
	Makesubdirs.bat modelicac-distclean
	Makesubdirs.bat intersci-distclean
	Makesubdirs.bat imp-distclean
	Makesubdirs.bat xmlint-distclean
	Makesubdirs.bat dumpexts-distclean

# win32 
LIBRSCI = libs/system.lib libs/console.lib libs/interf.lib \
	libs/system2.lib libs/optim.lib \
	libs/integ.lib libs/control.lib routines/control/fstair.obj \
	routines/control/wexchn.obj routines/control/wshrsl.obj \
	libs/scicos.lib libs/signal.lib \
	libs/poly.lib libs/calelm.lib libs/lapack.lib libs/slicot.lib \
	libs/graphics.lib \
	libs/sparse.lib libs/metanet.lib libs/os_specific.lib \
	libs/intersci.lib libs/wsci.lib libs/graphics.lib libs/menusX.lib \
	libs/fileio.lib libs/dcd.lib \
	libs/rand.lib libs/blas.lib libs/xdr.lib  \
	libs/gd.lib libs/int.lib libs/arpack.lib \
	libs/libf2c.lib libs/wintools.lib $(TCLSCI) $(PVM)

LIBR = $(XAW_LOCAL_LIB) $(LIBRSCI) $(DLDLIB)

DEFAULTS = routines/wsci/winmain.obj	\
	routines/f2c/libf2c/main.obj \
	routines/default/Ex-colnew.obj \
	routines/default/Ex-corr.obj \
	routines/default/Ex-feval.obj \
	routines/default/Ex-fsolve.obj \
        routines/default/Ex-lsqrsolve.obj \
	routines/default/Ex-impl.obj \
	routines/default/Ex-intg.obj \
	routines/default/Ex-int2d.obj \
	routines/default/Ex-int3d.obj \
	routines/default/Ex-ode-more.obj \
	routines/default/Ex-ode.obj\
	routines/default/Ex-odedc.obj \
	routines/default/Ex-optim.obj \
	routines/default/Ex-schur.obj \
	routines/default/Ex-fort.obj\
	routines/default/Ex-dasrt.obj \
	routines/default/Ex-dassl.obj \
	routines/default/Ex-fbutn.obj \
	routines/default/FTables.obj \
	routines/default/mainwin95.obj \
	routines/default/matusr.obj \
	routines/default/matus2.obj \
	routines/default/Funtab.obj \
	routines/default/msgstxt.obj \
	routines/default/scimem.obj \
	routines/default/callinterf.obj \
	routines/default/mexlib.obj

include config/Makefile.mak
