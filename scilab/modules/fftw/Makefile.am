# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Sylvestre LEDRU
# Copyright (C) 2007 - INRIA - Allan CORNET
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

modulename=fftw

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscifftw.la
noinst_LTLIBRARIES =
else
noinst_LTLIBRARIES = libscifftw.la
endif


if FFTW
noinst_LTLIBRARIES += libscifftw-algo.la
endif

libscifftw_la_LDFLAGS = $(AM_LDFLAGS)

#### fftw : Conf files ####
libscifftw_la_rootdir = $(mydatadir)

#### fftw : init scripts ####
libscifftw_la_etcdir = $(mydatadir)/etc
libscifftw_la_etc_DATA = \
    etc/fftw.quit \
    etc/fftw.start

if FFTW

FFTW_C_SOURCES = \
    src/c/callfftw.c \
    src/c/fftwlibname.c \
    src/c/fftw_utilities.c \
    src/c/with_fftw.c

FFTW_CPP_SOURCES = \
    src/cpp/fftw_common.cpp

GATEWAY_C_SOURCES = \
	sci_gateway/c/sci_fftwlibraryisloaded.c \
	sci_gateway/c/sci_disposefftwlibrary.c \
	sci_gateway/c/sci_fftw_flags.c \
	sci_gateway/c/sci_loadfftwlibrary.c \
	sci_gateway/c/sci_get_fftw_wisdom.c \
	sci_gateway/c/sci_set_fftw_wisdom.c \
	sci_gateway/c/sci_fftw_forget_wisdom.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/fftw_gw.cpp \
    sci_gateway/cpp/sci_fftw.cpp \
    sci_gateway/cpp/sci_dst.cpp \
    sci_gateway/cpp/sci_dct.cpp

#### fftw : Conf files ####
libscifftw_la_root_DATA =  license.txt

#### fftw : gateway declaration ####
#libscifftw_la_sci_gatewaydir = $(mydatadir)/sci_gateway
#libscifftw_la_sci_gateway_DATA = sci_gateway/fftw_gateway.xml

libscifftw_algo_la_SOURCES = $(FFTW_C_SOURCES) $(FFTW_CPP_SOURCES)
libscifftw_la_SOURCES = $(GATEWAY_C_SOURCES)  $(GATEWAY_CPP_SOURCES)
libscifftw_algo_la_CPPFLAGS = $(libscifftw_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(FFTW_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscifftw_la_CPPFLAGS)

libscifftw_la_LIBADD = libscifftw-algo.la $(FFTW3_LIB)

libscifftw_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(FFTW3_CFLAGS) \
    $(AM_CPPFLAGS)

else

FFTW_CPP_SOURCES = src/nofftw/nofftw.cpp

libscifftw_la_SOURCES = $(FFTW_CPP_SOURCES)

libscifftw_la_CPPFLAGS = \
-I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
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
    $(AM_CPPFLAGS)
endif

include $(top_srcdir)/Makefile.incl.am
