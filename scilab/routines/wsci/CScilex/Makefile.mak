SHELL = /bin/sh
SCIDIR=../../..
SCIDIR1=..\..\..
LIBRARY = $(SCIDIR)/libs/wsci.lib


OBJSC = CScilex.obj

OBJSF = 

include ../../../Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) 
FFLAGS = $(FC_OPTIONS)






#=================== Console Scilex ===========================

all:: ../../../bin/CScilex.exe

../../../bin/CScilex.exe 	: CScilex.obj
	 


CScilex.obj : CScilex.c 
