SHELL = /bin/sh
SCIDIR=../..

OBJSC = winDumpExts.o
OBJSF = 

include ../../Makefile.incl
CFLAGS = $(CC_OPTIONS) -mno-cygwin 
FFLAGS = $(FC_OPTIONS)

GUIFLAGS=-SUBSYSTEM:console

#=================== dumpexts.exe ===========================

all:: ../../bin/dumpexts.exe

../../bin/dumpexts.exe 	:  winDumpExts.o
	$(CC) $(CFLAGS) -o ../../bin/dumpexts.exe winDumpExts.o





