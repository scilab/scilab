PVM_ROOT=d:\Scilab\pvm3
PVM_ARCH=WIN32
PVMLIB=$(PVM_ROOT)\lib\WIN32\libpvm3.lib $(PVM_ROOT)\lib\WIN32\libgpvm3.lib 
PVM_CINCLUDE="."
PVM_INCLUDES=-I$(PVM_ROOT)\include -I$(PVM_ROOT)\src
PVM_CCOMPILER=VISUALC++


all	:: message libs pvmtracer pvmconsole gslib pvmhoster

message	:: 
	@echo ====================================
	@echo edit file $(PVM_ROOT)/conf/win32.def 
	@echo to fit your local compiler configuration 
	@echo ====================================

libs	:
	@echo making all in src 
	@cd $(PVM_ROOT)\src
	@SET PVM_ROOT=$(PVM_ROOT)
	@SET PVM_ARCH=$(PVM_ARCH)
	@SET PVM_CCOMPILER=$(PVM_CCOMPILER)
	@SET PVM_CINCLUDE=.
	@nmake /C /f makefile.mak
	@cd $(PVM_ROOT)

pvmtracer: 
	@echo making all in tracer 
	@cd $(PVM_ROOT)\tracer
	@SET PVM_ROOT=$(PVM_ROOT)
	@SET PVM_ARCH=$(PVM_ARCH)
	@SET PVM_CCOMPILER=$(PVM_CCOMPILER)
	@SET PVM_CINCLUDE=.
	@nmake /C /f makefile.mak
	@cd $(PVM_ROOT)

pvmconsole:
	@echo making all in console
	@cd $(PVM_ROOT)\console
	@SET PVM_ROOT=$(PVM_ROOT)
	@SET PVM_ARCH=$(PVM_ARCH)
	@SET PVM_CCOMPILER=$(PVM_CCOMPILER)
	@SET PVM_CINCLUDE=.
	@nmake /C /f makefile.mak
	@cd $(PVM_ROOT)

pvmhoster:
	@echo making all in hoster
	@cd $(PVM_ROOT)\hoster
	@SET PVM_ROOT=$(PVM_ROOT)
	@SET PVM_ARCH=$(PVM_ARCH)
	@SET PVM_CCOMPILER=$(PVM_CCOMPILER)
	@SET PVM_CINCLUDE=.
	@nmake /C /f makefile.mak
	@cd $(PVM_ROOT)

gslib	: 
	@echo making all in pvmgs
	@cd $(PVM_ROOT)\pvmgs
	@SET PVM_ROOT=$(PVM_ROOT)
	@SET PVM_ARCH=$(PVM_ARCH)
	@SET PVM_COMPILER=$(PVM_COMPILER)
	@SET PVM_CINCLUDE=.
	@nmake /C /f makefile.mak
	@cd $(PVM_ROOT)

clean ::
	@echo Cleaning Objects ...
	cd $(PVM_ROOT)\src\$(PVM_ARCH)
	erase *.obj
	cd $(PVM_ROOT)\pvmgs\$(PVM_ARCH)
	erase *.obj
	cd $(PVM_ROOT)\tracer\$(PVM_ARCH)
	erase *.obj
	cd $(PVM_ROOT)\console\$(PVM_ARCH)
	erase *.obj

distclean:: clean

