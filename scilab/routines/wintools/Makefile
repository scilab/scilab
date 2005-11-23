SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/wintools.a

OBJSC = intOEMconvert.o intmcisendstring.o intwintools.o intabout.o intclipboard.o intconsoledos.o intdde.o \
        intpathname.o intsetlanguagemenu.o inttoolbar.o \
        intwinopen.o intwinqueryreg.o intprintbox.o inttoprint.o intihmcolor.o intfileassociationbox.o
        
OBJSF =


include ../../Makefile.incl

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

intmcisendstring.o: ../stack.h intmcisendstring.h
intwintools.o: ../stack.h intwintools.h
intabout.o: ../stack.h intabout.h
intclipboard.o: ../stack.h intclipboard.h
intconsoledos.o: ../stack.h intconsoledos.h
intdde.o: ../stack.h intdde.h
intpathname.o: ../stack.h intpathname.h
intsetlanguagemenu.o: ../stack.h intsetlanguagemenu.h
inttoolbar.o: ../stack.h inttoolbar.h
intwinopen.o: ../stack.h intwinopen.h
intwinqueryreg.o: ../stack.h intwinqueryreg.h
