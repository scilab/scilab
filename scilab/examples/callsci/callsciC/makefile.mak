SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB=$(SCIDIR)/bin/LibScilab.lib

CFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"
CPPFLAGS= $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"

!include $(SCIDIR)/Makefile.incl.mak 


OBJSC=$(SCIDIR)/routines/f2c/libf2c/main.obj myprog.obj my_ode.obj

all:: $(OBJSC) $(SCIDIR)/bin/prog.exe 

distclean:: clean

clean	::
	@del *.obj 

distclean:: clean 
	@del $(SCIDIR1)\bin\prog.*


RESOURCES= $(SCIDIR)/routines/wsci/Rscilab.res 

$(SCIDIR)/routines/f2c/libf2c/main.obj :
	$(CC) /c ../../../routines/f2c/libf2c/main.c 
	@copy main.obj ..\..\..\routines\f2c\libf2c\main.obj 

$(SCIDIR)/bin/prog.exe : $(OBJSC)
	@echo "Linking" 
	$(LINKER) $(LINKER_FLAGS) -OUT:"$*.exe" \
	$(OBJSC) $(SCIDIR)/bin/LibScilab.lib $(XLIBSBIN) 
	@echo "done " $(SCIDIR)/bin/prog.exe 
