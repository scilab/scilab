SHELL = /bin/sh

SCIDIR=../../../../../..
SCIDIR1=..\..\..\..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

CFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/modules/core/includes" /EHsc
CPPFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/modules/core/includes" /EHsc

!include $(SCIDIR)/modules/dynamic_link/src/scripts/Makefile.incl.mak 

OBJSC = myprog.obj ccmatrix1.obj 

all:: $(OBJSC)  $(SCIDIR)/bin/prog.exe 

distclean:: clean

clean	::
	@del *.obj 
	@del *.lib
	@del *.dll
	@del *.exp
	@del *.def
	@del Makelib.mak
	@del loader.sce
	

distclean:: clean 
	@del $(SCIDIR1)\bin\prog.exe



$(SCIDIR)/bin/prog.exe : $(OBJSC)
	@echo "Linking" 
	$(LINKER) $(LINKER_FLAGS) -OUT:"$*.exe"  \
	$(OBJSC) $(SCIDIR)/bin/LibScilab.lib $(TKLIBS) $(GUI)  -NODEFAULTLIB:msvcrt.lib 
	@echo "done " $(SCIDIR)/bin/prog.exe 
