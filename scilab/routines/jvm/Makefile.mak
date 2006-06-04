SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/libjvm.lib

OBJSC = jvms.obj JVM_CONSOLE.obj JVM_Windows.obj


OBJSF = 


include ../../Makefile.incl.mak



CFLAGS = $(CC_OPTIONS) $(XFLAGS) -I"..\..\Java\jdk1.5.0_06\include" -I"..\..\Java\jdk1.5.0_06\include\Win32"

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak
