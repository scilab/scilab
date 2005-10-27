#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/intersci.lib

OBJSC = libinter.obj 
OBJSF = erro.obj out.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

erro.obj:       ../stack.h
out.obj:       ../stack.h

cdouble.obj: cdouble.c ../machine.h
cdoublef.obj: cdoublef.c ../machine.h
cerro.obj: cerro.c ../machine.h cerro.h
cout.obj: cout.c ../machine.h
libinter.obj: libinter.c ../machine.h ../graphics/Math.h \
	../graphics/Graphics.h ../os_specific/men_Sutils.h libinter.h cerro.h sparse.h \
	../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
	../interf/stack3.h

