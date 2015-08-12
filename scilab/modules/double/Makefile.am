# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

GATEWAY_C_SOURCES = sci_gateway/c/gw_double.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/vecldiv.f \
sci_gateway/fortran/vecmul.f \
sci_gateway/fortran/matxpow.f \
sci_gateway/fortran/matldiv.f \
sci_gateway/fortran/matsubt.f \
sci_gateway/fortran/vecrdiv.f \
sci_gateway/fortran/matchsgn.f \
sci_gateway/fortran/matrdiv.f \
sci_gateway/fortran/matrc.f \
sci_gateway/fortran/matpow.f \
sci_gateway/fortran/matins1.f \
sci_gateway/fortran/matins2.f \
sci_gateway/fortran/matcmp.f \
sci_gateway/fortran/matmult.f \
sci_gateway/fortran/matext1.f \
sci_gateway/fortran/matext2.f \
sci_gateway/fortran/mattr.f \
sci_gateway/fortran/matcc.f \
sci_gateway/fortran/mattrc.f \
sci_gateway/fortran/matadd.f

noinst_LTLIBRARIES = libdummy-double.la
if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscidouble.la
else
noinst_LTLIBRARIES += libscidouble.la
endif



libscidouble_la_CPPFLAGS = -I$(top_srcdir)/modules/api_scilab/includes/ \
-I$(top_srcdir)/modules/output_stream/includes/ \
$(AM_CPPFLAGS)

libscidouble_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscidouble_la_CPPFLAGS)

libscidouble_la_LIBADD = libdummy-double.la

libdummy_double_la_SOURCES = sci_gateway/fortran/vecimpl.f

# Following lines are extracted from a processed Makefile.in
# Take care while modifying : append -O0 after all commons options
vecimpl.lo: sci_gateway/fortran/vecimpl.f
	$(AM_V_F77)$(LIBTOOL) $(AM_V_lt) --tag=F77 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(F77) $(AM_FFLAGS) $(FFLAGS) -O0 -c -o vecimpl.lo `test -f 'sci_gateway/fortran/vecimpl.f' || echo '$(srcdir)/'`sci_gateway/fortran/vecimpl.f



#### Target ######
modulename=double


#### double : Conf files ####
libscidouble_la_rootdir = $(mydatadir)
libscidouble_la_root_DATA =  license.txt 


#### double : init scripts ####
libscidouble_la_etcdir = $(mydatadir)/etc
libscidouble_la_etc_DATA = etc/double.quit etc/double.start

include $(top_srcdir)/Makefile.incl.am
