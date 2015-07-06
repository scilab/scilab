# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

INTEGER_C_SOURCES = \
    src/c/genconcatcol.c \
    src/c/genprod.c \
    src/c/genrdiv.c \
    src/c/genextrac1.c \
    src/c/genextrac2.c \
    src/c/gencopy.c \
    src/c/gentril.c \
    src/c/gencheck.c \
    src/c/tpconv.c \
    src/c/genscal.c \
    src/c/gentriu.c \
    src/c/genvmul.c \
    src/c/genconcatrow.c \
    src/c/geninsert1.c \
    src/c/geninsert2.c \
    src/c/gendot.c \
    src/c/db2int.c \
    src/c/genadd.c \
    src/c/genset.c \
    src/c/gencusum.c \
    src/c/gensubst.c \
    src/c/genmdsp.c \
    src/c/gensum.c \
    src/c/genmmul.c \
    src/c/genmsum.c \
    src/c/genmprod.c \
    src/c/genmtran.c \
    src/c/genisany.c \
    src/c/genmcopy.c \
    src/c/gencuprod.c \
    src/c/genbitops.c \
    src/c/genimpl.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/integer_gw.cpp \
    sci_gateway/cpp/sci_inttype.cpp \
    sci_gateway/cpp/sci_double.cpp \
    sci_gateway/cpp/sci_int.cpp \
    sci_gateway/cpp/sci_iconvert.cpp

libsciinteger_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciinteger.la
noinst_LTLIBRARIES = libsciinteger-algo.la
else
noinst_LTLIBRARIES = libsciinteger-algo.la libsciinteger.la
endif


libsciinteger_algo_la_SOURCES = $(INTEGER_C_SOURCES)
libsciinteger_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libsciinteger_algo_la_CPPFLAGS = $(libsciinteger_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC=  $(INTEGER_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciinteger_la_CPPFLAGS)

# cyclic $(top_builddir)/modules/polynomials/libscipolynomials.la
libsciinteger_la_LIBADD = libsciinteger-algo.la



#### Target ######
modulename=integer


#### integer : Conf files ####
libsciinteger_la_rootdir = $(mydatadir)
libsciinteger_la_root_DATA =  license.txt


#### integer : init scripts ####
libsciinteger_la_etcdir = $(mydatadir)/etc
libsciinteger_la_etc_DATA = \
    etc/integer.quit \
    etc/integer.start

#### integer : gateway declaration ####
libsciinteger_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciinteger_la_sci_gateway_DATA = sci_gateway/integer_gateway.xml

include $(top_srcdir)/Makefile.incl.am
