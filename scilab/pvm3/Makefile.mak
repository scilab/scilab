

SHELL = /bin/sh

include ../Makefile.incl.mak

all	:: message libs  gslib

message	:: 
	@echo ====================================
	@echo edit file $(PVM_ROOT)/conf/win32.def 
	@echo to fit your local compiler configuration 
	@echo ====================================

libs	:
	cd $(PVM_ROOT)\src
	SET PVM_ROOT=$(PVM_ROOT)
	SET PVM_ARCH=$(PVM_ARCH)
	nmake /f makefile.mak
	cd $(PVM_ROOT)

gslib	: 
	cd $(PVM_ROOT)\pvmgs
	nmake /f makefile.mak
	cd $(PVM_ROOT)

clean ::
	echo Cleaning Objects ...
	cd $(PVM_ROOT)\src\$(PVM_ARCH)
	erase *.obj
	echo Cleaning pvmgs objects ...
	cd $(PVM_ROOT)\pvmgs\$(PVM_ARCH)
	erase *.obj

distclean:: clean

