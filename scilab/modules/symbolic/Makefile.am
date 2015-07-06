# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


FORTRAN_SOURCES = \
    src/fortran/atome.f \
    src/fortran/iseye.f \
    src/fortran/isnum.f \
    src/fortran/termf.f

libscisymbolic_la_CPPFLAGS = \
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
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscisymbolic.la
noinst_LTLIBRARIES = libscisymbolic-algo.la


libscisymbolic_algo_la_SOURCES = $(GATEWAY_FORTRAN_SOURCES) $(FORTRAN_SOURCES)
libscisymbolic_la_SOURCES = $(GATEWAY_C_SOURCES)
libscisymbolic_algo_la_CPPFLAGS = $(libscisymbolic_la_CPPFLAGS)
# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscisymbolic_la_CPPFLAGS)

libscisymbolic_la_LIBADD = libscisymbolic-algo.la



#### Target ######
modulename=symbolic


#### symbolic : Conf files ####
libscisymbolic_la_rootdir = $(mydatadir)
libscisymbolic_la_root_DATA =  license.txt


#### symbolic : init scripts ####
libscisymbolic_la_etcdir = $(mydatadir)/etc
libscisymbolic_la_etc_DATA = \
    etc/symbolic.quit \
    etc/symbolic.start

#### symbolic : gateway declaration ####
libscisymbolic_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisymbolic_la_sci_gateway_DATA = sci_gateway/symbolic_gateway.xml

include $(top_srcdir)/Makefile.incl.am
