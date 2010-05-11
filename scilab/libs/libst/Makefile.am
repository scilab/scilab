# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

LIBST_C_SOURCES = libst.c  misc.c


pkglib_LTLIBRARIES = libscilibst.la

libscilibst_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscilibst_la_SOURCES = $(LIBST_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(LIBST_C_SOURCES)

libscilibst_la_LIBADD =

#### Target ######
modulename=libst

include $(top_srcdir)/Makefile.incl.am
