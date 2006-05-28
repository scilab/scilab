SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/hashtable.lib

OBJSC = hashtable.obj


OBJSF = 


include ../../Makefile.incl.mak



CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
