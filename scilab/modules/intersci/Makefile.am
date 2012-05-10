# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


INTERSCI_LIB_C_SOURCES = \
src/lib/libinter.c \
src/lib/out.c

INTERSCI_EXE_C_SOURCES = \
src/exe/intersci-n.c \
src/exe/getrhs.c \
src/exe/crerhs.c \
src/exe/variables.c \
src/exe/read.c \
src/exe/fornames.c \
src/exe/out-e.c \
src/exe/outext.c \
src/exe/declare.c \
src/exe/opt.c \
src/exe/check.c

libsciintersci_la_includedir=$(pkgincludedir)
libsciintersci_la_include_HEADERS = includes/libinter.h

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciintersci.la
else
noinst_LTLIBRARIES = libsciintersci.la
endif



bin_PROGRAMS = intersci
intersci_SOURCES = $(INTERSCI_EXE_C_SOURCES)

libsciintersci_la_SOURCES = $(INTERSCI_LIB_C_SOURCES)

libsciintersci_la_CPPFLAGS = -I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(srcdir)/includes/ \
                -I$(top_srcdir)/modules/string/includes/ \
                $(AM_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libsciintersci_la_SOURCES)
INCLUDE_FLAGS = $(libsciintersci_la_CPPFLAGS)

#### Target ######
modulename=intersci


#### intersci : Conf files ####
libsciintersci_la_rootdir = $(mydatadir)
libsciintersci_la_root_DATA =  license.txt 


#### intersci : init scripts ####
libsciintersci_la_etcdir = $(mydatadir)/etc
libsciintersci_la_etc_DATA = etc/intersci.quit etc/intersci.start

include $(top_srcdir)/Makefile.incl.am

