SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/libdld.a

OBJSC = dld.o find_exec.o define.o get_func.o get_symbol.o \
	list_undef.o mk_dummy.o ref.o ul_file.o ul_symbol.o \
	remove.o error.o

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

define.o: defs.h dld.h
dld.o: defs.h dld.h
error.o: defs.h dld.h
get_func.o: defs.h dld.h
get_symbol.o: defs.h dld.h
list_undef.o: defs.h dld.h
mk_dummy.o: defs.h dld.h
ref.o: defs.h dld.h
remove.o: defs.h dld.h
ul_file.o: defs.h dld.h
ul_symbol.o: defs.h dld.h

