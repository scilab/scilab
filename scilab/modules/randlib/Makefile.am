# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
# Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
#
#
# This file is distributed under the same license as the Scilab package.


RANDLIB_C_SOURCES = \
    src/c/mt.c \
    src/c/igngeom.c \
    src/c/kiss.c \
    src/c/urand.c \
    src/c/clcg2.c \
    src/c/clcg4.c \
    src/c/sexpo.c \
    src/c/ignbin.c \
    src/c/ignpoi.c \
    src/c/sgamma.c \
    src/c/genbet.c 

RANDLIB_FORTRAN_SOURCES = \
    src/fortran/gennf.f \
    src/fortran/genchi.f \
    src/fortran/setgmn.f \
    src/fortran/gengam.f \
    src/fortran/snorm.f \
    src/fortran/gennch.f \
    src/fortran/genmn.f \
    src/fortran/phrtsd.f \
    src/fortran/ignnbn.f \
    src/fortran/genf.f \
    src/fortran/genunf.f \
    src/fortran/genexp.f \
    src/fortran/genmul.f \
    src/fortran/genprm.f \
    src/fortran/gennor.f 

RANDLIB_CPP_SOURCES = src/cpp/grand.cpp

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_grand.cpp

libscirandlib_la_CPPFLAGS = \
	-I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/localization/includes/ \
	-I$(top_srcdir)/modules/dynamic_link/includes/ \
	-I$(top_srcdir)/modules/threads/includes/ \
	-I$(top_srcdir)/modules/dynamic_link/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes/ \
	-I$(top_srcdir)/modules/string/includes/ \
	-I$(top_srcdir)/modules/console/includes/ \
	$(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscirandlib.la
noinst_LTLIBRARIES = libscirandlib-algo.la


libscirandlib_algo_la_SOURCES = $(RANDLIB_CPP_SOURCES) $(RANDLIB_C_SOURCES) $(RANDLIB_FORTRAN_SOURCES)
libscirandlib_la_SOURCES = $(GATEWAY_CPP_SOURCES) 
libscirandlib_algo_la_CPPFLAGS = $(libscirandlib_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(RANDLIB_CPP_SOURCES) $(RANDLIB_C_SOURCES) $(GATEWAY_CPP_SOURCES)
INCLUDE_FLAGS = $(libscirandlib_la_CPPFLAGS)

libscirandlib_la_LIBADD =  libscirandlib-algo.la




#### Target ######
modulename=randlib


#### randlib : Conf files ####
libscirandlib_la_rootdir = $(mydatadir)
libscirandlib_la_root_DATA =  license.txt 


#### randlib : init scripts ####
libscirandlib_la_etcdir = $(mydatadir)/etc
libscirandlib_la_etc_DATA = \
    etc/randlib.quit \
    etc/randlib.start

#### randlib : gateway declaration ####
libscirandlib_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscirandlib_la_sci_gateway_DATA = sci_gateway/randlib_gateway.xml

include $(top_srcdir)/Makefile.incl.am
