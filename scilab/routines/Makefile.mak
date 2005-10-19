SHELL = /bin/sh

include ../Makefile.incl.mak

SCIDIRS = graphics calelm control default integ interf intersci lapack \
	metanet optim poly signal sparse os_specific system system2 console\
	slicot  \
	menusX scicos fileio wsci xdr  gd int wsci/WScilex wsci/CScilex

SUBDIRS = $(XAW_LOCAL_SUBDIR) $(DLD_SUBDIR) $(SCIDIRS)

all::
	copy machine-h.vc machine.h
	copy version-h.vc version.h
	Makesubdirs.bat all



!IF "$(DTK)" == "-DWITH_TK"
all::
	Makesubdirs.bat tclsci
!ENDIF

!IF "$(DPVM)" == "-DWITH_PVM"
all::
	Makesubdirs.bat pvm
!ENDIF


clean::
	Makesubdirs.bat clean

distclean::
	Makesubdirs.bat distclean 
