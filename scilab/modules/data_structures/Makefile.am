# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/data_structures_gw.cpp \
    sci_gateway/cpp/sci_getfield.cpp \
    sci_gateway/cpp/sci_setfield.cpp \
    sci_gateway/cpp/sci_rlist.cpp \
    sci_gateway/cpp/sci_definedfields.cpp

libscidata_structures_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscidata_structures.la
else
noinst_LTLIBRARIES = libscidata_structures.la
endif

libscidata_structures_la_SOURCES = $(GATEWAY_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_CPP_SOURCES)
INCLUDE_FLAGS = $(libscidata_structures_la_CFLAGS)

#### Target ######
modulename=data_structures


#### data_structures : Conf files ####
libscidata_structures_la_rootdir = $(mydatadir)
libscidata_structures_la_root_DATA =  license.txt


#### data_structures : init scripts ####
libscidata_structures_la_etcdir = $(mydatadir)/etc
libscidata_structures_la_etc_DATA = \
    etc/data_structures.quit \
    etc/data_structures.start

#### data_structures : gateway declaration ####
libscidata_structures_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidata_structures_la_sci_gateway_DATA = sci_gateway/data_structures_gateway.xml

include $(top_srcdir)/Makefile.incl.am
