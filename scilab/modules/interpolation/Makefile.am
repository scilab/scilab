# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

# Localization of the module interpolation
# Please see in SCI/tools/localization for localization management
#

INTERPOLATION_FORTRAN_SOURCES = src/fortran/somespline.f \
src/fortran/dspfit.f \
src/fortran/cshep2d.f

INTERPOLATION_C_SOURCES = src/c/interpolation.c \
src/c/someinterp.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_lsq_splin.c \
sci_gateway/c/sci_interp.c \
sci_gateway/c/sci_interp2d.c \
sci_gateway/c/sci_splin.c \
sci_gateway/c/gw_interpolation.c \
sci_gateway/c/sci_cshep2d.c \
sci_gateway/c/sci_interp3d.c \
sci_gateway/c/sci_linear_interpn.c \
sci_gateway/c/sci_splin2d.c \
sci_gateway/c/sci_splin3d.c \
sci_gateway/c/sci_bsplin3val.c \
sci_gateway/c/sci_eval_cshep2d.c

libsciinterpolation_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
                -I$(top_srcdir)/modules/string/includes/ \
                $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libsciinterpolation.la
noinst_LTLIBRARIES = libsciinterpolation-algo.la


libsciinterpolation_algo_la_SOURCES = $(INTERPOLATION_C_SOURCES) $(INTERPOLATION_FORTRAN_SOURCES)
libsciinterpolation_la_SOURCES = $(GATEWAY_C_SOURCES)
libsciinterpolation_algo_la_CPPFLAGS = $(libsciinterpolation_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(INTERPOLATION_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciinterpolation_la_CPPFLAGS)

libsciinterpolation_la_LIBADD = \
	libsciinterpolation-algo.la

#### Target ######
modulename=interpolation


#### interpolation : Conf files ####
libsciinterpolation_la_rootdir = $(mydatadir)
libsciinterpolation_la_root_DATA =  license.txt 


####�interpolation : init scripts ####
libsciinterpolation_la_etcdir = $(mydatadir)/etc
libsciinterpolation_la_etc_DATA = etc/interpolation.quit etc/interpolation.start

####�interpolation : gateway declaration ####
libsciinterpolation_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciinterpolation_la_sci_gateway_DATA = sci_gateway/interpolation_gateway.xml

include $(top_srcdir)/Makefile.incl.am
