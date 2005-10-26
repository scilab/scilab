SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/intersci.a

OBJSC = libinter.o 
OBJSF = erro.o out.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

erro.o:       ../stack.h
out.o:       ../stack.h

cdouble.o: cdouble.c ../machine.h
cdoublef.o: cdoublef.c ../machine.h
cerro.o: cerro.c ../machine.h cerro.h
cout.o: cout.c ../machine.h
libinter.o: libinter.c ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h ../os_specific/men_Sutils.h libinter.h cerro.h sparse.h \
  ../stack-c.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h
  