SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/fileio.lib

RIPOLE= bt-int.obj bytedecoders.obj logger.obj ole.obj olestream-unwrap.obj pldstr.obj ripole.obj

OBJSC = matfileio.obj intfileio.obj sound.obj misc.obj fileio.obj xls.obj $(RIPOLE)

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

