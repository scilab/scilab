SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

CFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines" /EHsc
CPPFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines" /EHsc

!include $(SCIDIR)/Makefile.incl.mak 

OBJSC = myprog.obj ccmatrix1.obj 

all:: $(OBJSC)  $(SCIDIR)/bin/prog.exe 

distclean:: clean

clean	::
	@del *.obj 

distclean:: clean 
	@del $(SCIDIR1)\bin\prog.*
	@del libodeex.* loader.sce Makelib.mak


$(SCIDIR)/bin/prog.exe : $(OBJSC)
	@echo "Linking" 
	$(LINKER) $(LINKER_FLAGS) -OUT:"$*.exe"  \
	$(OBJSC) $(SCIDIR)/bin/LibScilab.lib $(TKLIBS) $(PVMLIB) -NODEFAULTLIB:libcmt.lib $(GUI) libc.lib  -NODEFAULTLIB:msvcrt.lib 
	@echo "done " $(SCIDIR)/bin/prog.exe 
