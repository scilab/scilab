# Makefile.mak used to generate dlls 
# for dem01.dem file or other files 
# This makefile can be used to create 
# dll's for files which are in other directory 
# nmake /f c:/.../demos/intro/Makefile.mak ctarget TARGET=foo SCIDIR1=c:\...\

SCIDIR=../..
SCIDIR1=..\..

DUMPEXTS="$(SCIDIR1)\bin\dumpexts"
SCIIMPLIB="$(SCIDIR1)\bin\LibScilab.lib"
F2C="$(SCIDIR1)\bin\f2c"

!include $(SCIDIR1)\Makefile.incl.mak 

FFLAGS = $(FC_OPTIONS) 
CFLAGS = $(CC_OPTIONS) -DFORDLL  -I"$(SCIDIR)/routines/f2c"
TARGET=foo 

all :: 

distclean:: clean

clean:: 
	-@del foo.* fooc.* 

