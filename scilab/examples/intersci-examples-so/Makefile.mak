.SUFFIXES: .desc $(SUFFIXES)

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS=$(SCIDIR1)\bin\dumpexts
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib
INTERSCI=$(SCIDIR1)\bin\intersci-n
F2C=$(SCIDIR1)\bin\f2c.exe

include ../../Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) -DFORDLL
CFLAGS = $(CC_OPTIONS) -DFORDLL 

all:: info

info:
	@echo ------------------------------------------;
	@echo At Scilab prompt, enter:;
	@echo -->exec exXX.sce; 
	@echo to execute example XX 
	@echo ------------------------------------------;
	@echo Type nmake /f Makefile.mak tests 
	@echo to run all tests 

.desc.c: 
	@echo  generating $*.c 
	@$(INTERSCI) $* 

clean::

distclean	::
	@del *.obj 
	@del *.dll
	@del *.ilib 
	@del *.pdk
	@del *.pdb
	@del *.ilk 
	@del *.def
	@del *.exp 
	@del *_builder.sce
	@del *.tmp
	@del *fi.c 
	@del libex*

tests	: 
	$(SCIDIR1)\bin\scilex.exe  -f zall.sce

distclean:: clean

clean	::
	@del zallfi
	@del zallfi.sce 
	@del zallfi.f 
	@del zallfi.obj 
	@del zallfi.dia

