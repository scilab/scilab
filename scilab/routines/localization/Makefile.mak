SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/localization.lib

OBJSC = localization.obj


OBJSF = 


include ../../Makefile.incl.mak



CFLAGS = $(CC_OPTIONS) $(XFLAGS) -I"..\libxml2" -I"..\glib-2.0"

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
