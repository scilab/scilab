SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/wintools.lib

OBJSC = intOEMconvert.obj intmcisendstring.obj intwintools.obj intabout.obj intclipboard.obj intconsoledos.obj intdde.obj \
        intpathname.obj intsetlanguagemenu.obj inttoolbar.obj \
        intwinopen.obj intwinqueryreg.obj intprintbox.obj inttoprint.obj \
        intihmcolor.obj intfileassociationbox.obj


OBJSF = 


include ../../Makefile.incl.mak

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
