SHELL = /bin/sh
SCIDIR=..
SCIDIR1=..

include ../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)


all::browsehelpexe.obj
	$(CC) browsehelpexe.c $(CC_OPTIONS) $(TCL_INCLUDES)
	$(LINKER) $(LINKER_FLAGS) \
		/out:browsehelpexe.exe browsehelpexe.obj $(TKLIBS) 
clean::
	-del *.obj
	-del browsehelpexe.exe
