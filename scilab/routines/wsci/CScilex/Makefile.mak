SHELL = /bin/sh
SCIDIR=../../..
SCIDIR1=..\..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib


OBJSC = CScilex_f2c.obj

OBJSF = 

include ../../../Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)






#=================== Console Scilex ===========================

all:: ../../../bin/CScilex.exe

../../../bin/CScilex.exe 	: CScilex_f2c.obj
	 


CScilex_f2c.obj : CScilex_f2c.c 
