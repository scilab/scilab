SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/libjvm.a

OBJSC = jvms.o JVM_CONSOLE.o JVM_UNIX.o
        
OBJSF =


include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) -I"../../Java/jdk1.5.0_06/include" -I"../../Java/jdk1.5.0_06/include/linux"

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

