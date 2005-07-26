SHELL = /bin/sh
SCIDIR=../../..
SCIDIR1=..\..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib


OBJSC = WScilex_f2c.obj

OBJSF = 

include ../../../Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)






#=================== Windows Scilex ===========================

all:: ../../../bin/WScilex.exe

../../../bin/WScilex.exe 	: WScilex_f2c.obj
	 


WScilex_f2c.obj : WScilex_f2c.c 
