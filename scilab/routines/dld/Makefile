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

define.o: define.c defs.h dld.h
dld.o: dld.c defs.h dld.h
error.o: error.c defs.h dld.h
find_exec.o: find_exec.c
get_func.o: get_func.c defs.h dld.h
get_symbol.o: get_symbol.c defs.h dld.h
list_undef.o: list_undef.c defs.h dld.h
mk_dummy.o: mk_dummy.c defs.h dld.h
ref.o: ref.c defs.h dld.h
remove.o: remove.c defs.h dld.h
ul_file.o: ul_file.c defs.h dld.h
ul_symbol.o: ul_symbol.c defs.h dld.h
