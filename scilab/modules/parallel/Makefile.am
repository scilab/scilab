# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2010 - DIGITEO - Allan CORNET
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

#PARALLEL_CPP_SOURCES =

#GATEWAY_CPP_SOURCES =
#sci_gateway/cpp/sci_parallel_run.cpp

#PARALLEL_C_SOURCES =

libsciparallel_la_CPPFLAGS= \
    $(OPENMP_CXXFLAGS) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/parameters/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes

pkglib_LTLIBRARIES = libsciparallel.la

libsciparallel_la_LDFLAGS = $(OPENMP_LIBS) -version-number $(SCILAB_LIBRARY_VERSION)


if OPENMP

libsciparallel_la_SOURCES = $(GATEWAY_CPP_SOURCES)

else
libsciparallel_la_SOURCES = src/noparallel/noparallel.h \
src/noparallel/noparallel.c
endif

# For the code check (splint)
CHECK_SRC= $(PARALLEL_C_SOURCES)
INCLUDE_FLAGS = $(libsciparallel_la_CFLAGS)

#### Target ######
modulename=parallel

#### parallel : Conf files ####
libsciparallel_la_rootdir = $(mydatadir)
libsciparallel_la_root_DATA = license.txt


#### parallel : init scripts ####
libsciparallel_la_etcdir = $(mydatadir)/etc
libsciparallel_la_etc_DATA = \
    etc/parallel.quit \
    etc/parallel.start

#### parallel : gateway declaration ####
libsciparallel_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciparallel_la_sci_gateway_DATA = sci_gateway/parallel_gateway.xml

#### parallel : include files ####
libsciparallel_la_includedir=$(pkgincludedir)
libsciparallel_la_include_HEADERS =

include $(top_srcdir)/Makefile.incl.am

