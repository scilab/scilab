SHELL = /bin/sh

include ../Makefile.incl.mak

SCIDIRS = graphics calelm control default integ interf intersci lapack \
	metanet optim poly signal sparse sun system system2 \
	slicot  \
	menusX scicos sound wsci xdr  gd int

SUBDIRS = $(XAW_LOCAL_SUBDIR) $(DLD_SUBDIR) $(SCIDIRS)

all::
	copy machine-h.vc machine.h
	copy version-h.vc version.h
	Makesubdirs.bat all



!IF "$(DTK)" == "-DWITH_TK"
all::
	Makesubdirs.bat tksci
!ENDIF

!IF "$(DPVM)" == "-DWITH_PVM"
all::
	Makesubdirs.bat pvm
!ENDIF


clean::
	Makesubdirs.bat clean

distclean::
	Makesubdirs.bat distclean 
