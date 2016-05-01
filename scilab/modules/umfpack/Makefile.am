# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
# Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

modulename=umfpack

pkglib_LTLIBRARIES = libsciumfpack.la

if UMFPACK
noinst_LTLIBRARIES = libsciumfpack-algo.la
endif

libsciumfpack_la_LDFLAGS = $(AM_LDFLAGS)

#### umfpack : Conf files ####
libsciumfpack_la_rootdir = $(mydatadir)
libsciumfpack_la_root_DATA =  \
license.txt \
TAUCS_license.txt \
UMFPACK_license.txt

#### umfpack : init scripts ####
libsciumfpack_la_etcdir = $(mydatadir)/etc
libsciumfpack_la_etc_DATA = \
    etc/umfpack.quit \
    etc/umfpack.start

if UMFPACK

UMFPACK_C_SOURCES = \
    src/c/taucs_scilab.c \
    src/c/common_umfpack.c \
    src/c/with_umfpack.c

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_res_with_prec.c \
    sci_gateway/c/sci_taucs_chdel.c \
    sci_gateway/c/sci_taucs_chfact.c \
    sci_gateway/c/sci_taucs_chget.c \
    sci_gateway/c/sci_taucs_chinfo.c \
    sci_gateway/c/sci_taucs_chsolve.c \
    sci_gateway/c/sci_umf_ludel.c \
    sci_gateway/c/sci_umf_lufact.c \
    sci_gateway/c/sci_umf_luget.c \
    sci_gateway/c/sci_umf_luinfo.c \
    sci_gateway/c/sci_umf_lusolve.c \
    sci_gateway/c/sci_umfpack.c

#### umfpack : gateway declaration ####
libsciumfpack_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciumfpack_la_sci_gateway_DATA = sci_gateway/umfpack_gateway.xml

libsciumfpack_algo_la_SOURCES = $(UMFPACK_C_SOURCES)
libsciumfpack_la_SOURCES = $(GATEWAY_C_SOURCES)
libsciumfpack_algo_la_CPPFLAGS = $(libsciumfpack_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libsciumfpack_la_SOURCES)
INCLUDE_FLAGS = $(libsciumfpack_la_CPPFLAGS)

libsciumfpack_la_LIBADD =  libsciumfpack-algo.la

libsciumfpack_la_LIBADD += $(UMFPACK_LIB)

if IS_MACOSX
# Crappy workaround to avoid an undefined symbol while using umfpack features
# For some unknown reasons, -lumfpack will not load cholmod & colamd
# libsciumfpack_la_LIBADD += -lcholmod -lcolamd
endif

libsciumfpack_la_CPPFLAGS= \
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
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/mexlib/includes/ \
    $(UMFPACK_CFLAGS) \
    $(AM_CPPFLAGS)

else

UMFPACK_C_SOURCES = src/noumfpack/noumfpack.c

libsciumfpack_la_SOURCES = $(UMFPACK_C_SOURCES)
libsciumfpack_la_CPPFLAGS= \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/umfpack/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    $(UMFPACK_CFLAGS) \
    $(AM_CPPFLAGS)
endif

include $(top_srcdir)/Makefile.incl.am
