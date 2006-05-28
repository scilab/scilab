SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/hashtable.lib

OBJSC = hashtable.obj


OBJSF = 


include ../../Makefile.incl.mak



CFLAGS = $(CC_OPTIONS) $(XFLAGS) -I"..\glib-2.0"

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
