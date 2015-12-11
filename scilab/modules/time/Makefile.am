# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

### SOURCES ###
TIME_C_SOURCES = \
    src/c/realtime.c \
    src/c/timer.c \
    src/c/getdate.c

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_realtime.c \
    sci_gateway/c/sci_timer.c \
    sci_gateway/c/sci_calendar.c \
    sci_gateway/c/sci_xpause.c \
    sci_gateway/c/sci_sleep.c \
    sci_gateway/c/sci_getdate.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/time_gw.cpp \
    sci_gateway/cpp/sci_tictoc.cpp

libscitime_la_CPPFLAGS= \
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
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscitime.la
noinst_LTLIBRARIES = libscitime-algo.la
else
noinst_LTLIBRARIES = libscitime-algo.la libscitime.la
endif


libscitime_algo_la_SOURCES = $(TIME_C_SOURCES)
libscitime_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscitime_algo_la_CPPFLAGS = $(libscitime_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscitime_la_SOURCES)
INCLUDE_FLAGS = $(libscitime_la_CPPFLAGS)

libscitime_la_LIBADD = libscitime-algo.la



#### Target ######
modulename=time


#### time : Conf files ####
libscitime_la_rootdir = $(mydatadir)
libscitime_la_root_DATA =  license.txt


#### time : init scripts ####
libscitime_la_etcdir = $(mydatadir)/etc
libscitime_la_etc_DATA = etc/time.quit etc/time.start

#### time : gateway declaration ####
libscitime_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitime_la_sci_gateway_DATA = sci_gateway/time_gateway.xml

#### time : include files ####
libscitime_la_includedir=$(pkgincludedir)
libscitime_la_include_HEADERS = includes/realtime.h \
includes/timer.h


include $(top_srcdir)/Makefile.incl.am
