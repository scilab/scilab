# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2010 - DIGITEO - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

#PARALLEL_CXX_SOURCES =

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_parallel_run.cpp

#PARALLEL_C_SOURCES =

GATEWAY_C_SOURCES = sci_gateway/c/gw_parallel.c \
sci_gateway/c/sci_parallel_concurrency.c

libsciparallel_la_CPPFLAGS= $(OPENMP_CPPFLAGS) \
-I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/core/includes/ \
-I$(srcdir)/src/cpp/ \
-I$(top_srcdir)/modules/api_scilab/includes/ \
-I$(top_srcdir)/modules/localization/includes/ \
-I$(top_srcdir)/modules/string/includes/ \
-I$(top_srcdir)/modules/output_stream/includes/ \
-I$(top_srcdir)/modules/parameters/includes/ \
-I$(top_srcdir)/modules/dynamic_link/includes/ \
$(AM_CPPFLAGS)


pkglib_LTLIBRARIES = libsciparallel.la



if OPENMP
libsciparallel_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)

else
libsciparallel_la_SOURCES = src/noparallel/noparallel.h \
src/noparallel/noparallel.c
endif

# For the code check (splint)
CHECK_SRC= $(PARALLEL_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciparallel_la_CPPFLAGS)

#### Target ######
modulename=parallel

#### parallel : Conf files ####
libsciparallel_la_rootdir = $(mydatadir)
libsciparallel_la_root_DATA = license.txt 


#### parallel : init scripts ####
libsciparallel_la_etcdir = $(mydatadir)/etc
libsciparallel_la_etc_DATA = etc/parallel.quit etc/parallel.start

#### parallel : gateway declaration ####
libsciparallel_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciparallel_la_sci_gateway_DATA = sci_gateway/parallel_gateway.xml

#### parallel : include files ####
libsciparallel_la_includedir=$(pkgincludedir)
libsciparallel_la_include_HEADERS =

include $(top_srcdir)/Makefile.incl.am


