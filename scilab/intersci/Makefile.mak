SHELL = /bin/sh

SCIDIR=..
include ../Makefile.incl.mak

# just add -DEPSFIG 
# il you prefer epsfig to special to insert postscript files in LaTeX 

CFLAGS = $(CC_OPTIONS)

all:: ..\bin\intersci.exe  ..\bin\intersci-n.exe

..\bin\intersci.exe: intersci.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" intersci.obj $(GUILIBS)

OBJ1 = intersci-n.obj getrhs.obj crerhs.obj variables.obj read.obj fornames.obj out.obj outext.obj declare.obj opt.obj check.obj

..\bin\intersci-n.exe: $(OBJ1)
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" $(OBJ1) $(GUILIBS)

clean::
	@del *.obj
	@del ..\bin\intersci.exe ..\bin\intersci-n.exe

distclean::  
	@del *.obj	
