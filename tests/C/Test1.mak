SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

CFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"
CPPFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"

!include $(SCIDIR)/Makefile.incl.mak 


OBJSC=Test1.obj

all:: $(OBJSC) $(SCIDIR)/bin/Test1.exe 

distclean:: clean

clean	::
	@del *.obj 

distclean:: clean 
	@del $(SCIDIR1)\bin\prog.*


RESOURCES= $(SCIDIR)/routines/wsci/Rscilab.res 

$(SCIDIR)/routines/f2c/libf2c/main.obj :
	$(CC) /c ../../routines/f2c/libf2c/main.c 
	@copy main.obj ..\..\routines\f2c\libf2c\main.obj 

$(SCIDIR)/bin/Test1.exe : $(OBJSC)
	@echo "Linking" 
	$(LINKER) $(LINKER_FLAGS) -OUT:"$*.exe" \
	$(OBJSC) $(SCIDIR)/bin/LibScilab.lib $(XLIBS) 
	@echo "done " $(SCIDIR)/bin/Test1.exe 
