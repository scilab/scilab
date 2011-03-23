# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

HASHTABLE_C_SOURCES = hashtable_utility.c \
hashtable_itr.c \
hashtable.c

libscihashtable_la_includedir=$(pkgincludedir)

libscihashtable_la_include_HEADERS = ./hashtable_itr.h \
./hashtable.h \
./hashtable_utility.h

pkglib_LTLIBRARIES = libscihashtable.la

libscihashtable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libscihashtable_la_SOURCES = $(HASHTABLE_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(HASHTABLE_C_SOURCES)

libscihashtable_la_LIBADD = 

#### Target ######
modulename=hashtable

include $(top_srcdir)/Makefile.incl.am