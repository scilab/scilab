# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
#
# This file is distributed under the same license as the Scilab package.


### SOURCES ###
CALLSCILAB_C_SOURCES = \
    src/c/call_scilab.c \
    src/c/fromc.c \
    src/c/fromjava.c \
    src/c/SendScilabJobs.c

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_fromjava.c \
    sci_gateway/c/sci_fromc.c

libscicall_scilab_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/action_binding/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/graphic_objects/includes/ \
    -I$(top_srcdir)/modules/core/src/c/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscicall_scilab.la
noinst_LTLIBRARIES = libscicall_scilab-algo.la


libscicall_scilab_algo_la_SOURCES = $(CALLSCILAB_C_SOURCES)
libscicall_scilab_la_SOURCES = $(GATEWAY_C_SOURCES)
libscicall_scilab_algo_la_CPPFLAGS = $(libscicall_scilab_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscicall_scilab_la_SOURCES)
INCLUDE_FLAGS = $(libscicall_scilab_la_CPPFLAGS)

libscicall_scilab_la_LIBADD = libscicall_scilab-algo.la




#### Target ######
modulename=call_scilab


#### call_scilab : Conf files ####
libscicall_scilab_la_rootdir = $(mydatadir)
libscicall_scilab_la_root_DATA =  license.txt


#### call_scilab : init scripts ####
libscicall_scilab_la_etcdir = $(mydatadir)/etc
libscicall_scilab_la_etc_DATA = etc/call_scilab.quit etc/call_scilab.start

#### call_scilab : gateway declaration ####
libscicall_scilab_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicall_scilab_la_sci_gateway_DATA = sci_gateway/call_scilab_gateway.xml

#### call_scilab : include files ####
libscicall_scilab_la_includedir=$(pkgincludedir)
libscicall_scilab_la_include_HEADERS = includes/call_scilab.h

include $(top_srcdir)/Makefile.incl.am
