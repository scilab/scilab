SHELL = /bin/sh

SCIDIR=../../../../../..
SCIDIR1=..\..\..\..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

CFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"
CPPFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"

!include $(SCIDIR)/modules/dynamic_link/src/scripts/Makefile.incl.mak 


OBJSC= myprog.obj

all:: $(OBJSC) $(SCIDIR)/bin/prog.exe 

distclean:: clean

clean	::
	@del *.obj 

distclean:: clean 
	@del $(SCIDIR1)\bin\prog.*


RESOURCES= $(SCIDIR)/routines/wsci/Rscilab.res 

$(SCIDIR)/bin/prog.exe : $(OBJSC)
	@echo "Linking" 
	$(LINKER) $(LINKER_FLAGS) -OUT:"$*.exe" \
	$(OBJSC) $(SCIDIR)/bin/LibScilab.lib $(SCILAB_LIBS) 
	@echo "done " $(SCIDIR)/bin/prog.exe 
