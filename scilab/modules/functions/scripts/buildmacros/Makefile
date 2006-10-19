SHELL = /bin/sh

#include ../Path.incl
#include ../Makefile.incl

#CFLAGS = $(CC_OPTIONS) 

all:: 
	../../../../.libs/lt-scilex -nwni -ns -f buildmacros.sce

release : 
	../bin/scilab -nwni -ns -f buildmacros.sce
distclean::
