# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Sylvestre LEDRU
# Copyright (C) 2007 - INRIA - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
libscifftw_la_etc_DATA = etc/fftw.quit etc/fftw.start

if FFTW

FFTW_C_SOURCES = src/c/callfftw.c \
src/c/fftwlibname.c \
src/c/fftw_utilities.c \
src/c/sci_data_utilities.c \
src/c/with_fftw.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_fftw.c \
	sci_gateway/c/sci_fftwlibraryisloaded.c \
	sci_gateway/c/sci_disposefftwlibrary.c \
	sci_gateway/c/sci_fftw.c \
	sci_gateway/c/sci_fftw_flags.c \
	sci_gateway/c/sci_loadfftwlibrary.c \
	sci_gateway/c/sci_get_fftw_wisdom.c \
	sci_gateway/c/sci_set_fftw_wisdom.c \
	sci_gateway/c/sci_fftw_forget_wisdom.c \
	sci_gateway/c/sci_dct.c \
	sci_gateway/c/sci_dst.c 

#### fftw : Conf files ####
libscifftw_la_root_DATA =  license.txt 

#### fftw : gateway declaration ####
libscifftw_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifftw_la_sci_gateway_DATA = sci_gateway/fftw_gateway.xml

libscifftw_algo_la_SOURCES = $(FFTW_C_SOURCES)
libscifftw_la_SOURCES = $(GATEWAY_C_SOURCES)
libscifftw_algo_la_CPPFLAGS = $(libscifftw_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(FFTW_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscifftw_la_CPPFLAGS)

libscifftw_la_LIBADD = libscifftw-algo.la $(FFTW3_LIB)

libscifftw_la_CPPFLAGS = -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/modules/dynamic_link/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			$(FFTW3_CFLAGS) \
			$(AM_CPPFLAGS)

else

FFTW_C_SOURCES = src/nofftw/nofftw.c

libscifftw_la_SOURCES = $(FFTW_C_SOURCES)

libscifftw_la_CPPFLAGS = -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			$(AM_CPPFLAGS)
endif

include $(top_srcdir)/Makefile.incl.am
