
SCIDIR=.
SCIDIR1=.

include Makefile.incl.mak 

all ::  routines-pvm  bin/scilex.exe bin/Wscilex.exe macros MakeMexLib imp intersci tcl

!IF "$(DLPVM)" == "YES"
routines-pvm :: routines pvm 
!ELSE 
routines-pvm :: routines 
!ENDIF

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

man::
	Makesubdirs.bat man 

tcl:: 
	Makesubdirs.bat tcl 

MakeMexLib::
	Makesubdirs.bat MakeMexLib

clean::
	Makesubdirs.bat routines-clean
	Makesubdirs.bat macros-clean

distclean::
	Makesubdirs.bat routines-distclean 
	Makesubdirs.bat macros-distclean 
	Makesubdirs.bat man-distclean 

# win32 
LIBRSCI = libs/system.lib libs/console.lib libs/interf.lib \
	libs/system2.lib libs/optim.lib \
	libs/integ.lib libs/control.lib routines/control/fstair.obj \
	routines/control/wexchn.obj routines/control/wshrsl.obj \
	libs/scicos.lib libs/signal.lib \
	libs/poly.lib libs/calelm.lib libs/lapack.lib libs/slicot.lib \
	libs/graphics.lib \
	libs/sparse.lib libs/metanet.lib libs/sun.lib \
	libs/intersci.lib libs/wsci.lib libs/graphics.lib libs/menusX.lib \
	libs/sound.lib libs/dcd.lib \
	libs/rand.lib libs/blas.lib libs/xdr.lib  \
	libs/gd.lib libs/int.lib libs/arpack.lib \
	libs/libf2c.lib $(TKSCI) $(PVM)

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
