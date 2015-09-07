# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
# Copyright (C) 2012 - Scilab Enterprises- Cedric Delamarre
#
# This file is distributed under the same license as the Scilab package.

ARNOLDI_C_SOURCES = \
    src/c/eigs.c \
    src/c/eigs_dependencies.c

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_zneupd \
    sci_gateway/c/sci_dseupd.c \
    sci_gateway/c/sci_zneupd.c \
    sci_gateway/c/sci_dnaupd.c \
    sci_gateway/c/sci_dneupd.c \
    sci_gateway/c/sci_dsaupd.c \
    sci_gateway/c/sci_znaupd.c \
    sci_gateway/c/sci_eigs.c

libsciarnoldi_la_CPPFLAGS = \
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
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libsciarnoldi.la

libsciarnoldi_la_LDFLAGS = $(AM_LDFLAGS)
libsciarnoldi_la_LIBADD = $(BLAS_LIBS) $(LAPACK_LIBS) $(ARPACK_LIBS)

libsciarnoldi_la_SOURCES = $(ARNOLDI_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(ARNOLDI_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciarnoldi_la_CPPFLAGS)

#### Target ######
modulename=arnoldi


#### arnoldi : Conf files ####
libsciarnoldi_la_rootdir = $(mydatadir)
libsciarnoldi_la_root_DATA =  license.txt


#### arnoldi : init scripts ####
libsciarnoldi_la_etcdir = $(mydatadir)/etc
libsciarnoldi_la_etc_DATA = etc/arnoldi.quit etc/arnoldi.start

#### arnoldi : gateway declaration ####
libsciarnoldi_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciarnoldi_la_sci_gateway_DATA = sci_gateway/arnoldi_gateway.xml

include $(top_srcdir)/Makefile.incl.am
