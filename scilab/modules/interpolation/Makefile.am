# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# 
# This file is distributed under the same license as the Scilab package.

# Localization of the module interpolation
# Please see in SCI/tools/localization for localization management
#

INTERPOLATION_FORTRAN_SOURCES = src/fortran/somespline.f \
src/fortran/dspfit.f \
src/fortran/cshep2d.f \
src/fortran/dtensbs.f \
src/fortran/pchim.f

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

libsciinterpolation_la_FFLAGS=  -Iincludes/
libsciinterpolation_la_CFLAGS= 	-Iincludes/ \
				-Isrc/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
                -I$(top_srcdir)/modules/string/includes

pkglib_LTLIBRARIES = libsciinterpolation.la

libsciinterpolation_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciinterpolation_la_SOURCES = $(INTERPOLATION_C_SOURCES) $(INTERPOLATION_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

libsciinterpolation_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
    $(top_builddir)/modules/string/libscistring.la
    

#### Target ######
modulename=interpolation


#### interpolation : Conf files ####
libsciinterpolation_la_rootdir = $(mydatadir)
libsciinterpolation_la_root_DATA = changelog.txt license.txt readme.txt version.xml


####�interpolation : init scripts ####
libsciinterpolation_la_etcdir = $(mydatadir)/etc
libsciinterpolation_la_etc_DATA = etc/interpolation.quit etc/interpolation.start

####�interpolation : gateway declaration ####
libsciinterpolation_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciinterpolation_la_sci_gateway_DATA = sci_gateway/interpolation_gateway.xml

####�interpolation : include files ####
libsciinterpolation_la_includedir=$(pkgincludedir)/interpolation
libsciinterpolation_la_include_HEADERS = includes/gw_interpolation.h 


####�interpolation : demos files ####
libsciinterpolation_la_demosdir=$(mydatadir)/demos/
libsciinterpolation_la_demos_DATA = demos/interp_demo.sci

include $(top_srcdir)/Makefile.incl.am
