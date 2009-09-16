##########
### Allan CORNET
### INRIA - Scilab 2007 
##########

DYNAMICLIBRARY_C_SOURCES = src/dynamiclibrary_others.c

libscidynamiclibrary_la_includedir=$(pkgincludedir)

libscidynamiclibrary_la_include_HEADERS = includes/dynamiclibrary_others.h \
includes/dynamiclibrary.h

libscidynamiclibrary_la_CFLAGS= -Isrc/ -I$(srcdir)/includes/

pkglib_LTLIBRARIES = libscidynamiclibrary.la

libscidynamiclibrary_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidynamiclibrary_la_SOURCES = $(DYNAMICLIBRARY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(DYNAMICLIBRARY_C_SOURCES)
INCLUDE_FLAGS = $(libscidynamiclibrary_la_CFLAGS)

libscidynamiclibrary_la_LIBADD = 

#### Target ######
modulename=dynamiclibrary

include $(top_srcdir)/Makefile.incl.am
