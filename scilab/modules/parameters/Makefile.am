# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

### SOURCES ###

PARAMETERS_C_SOURCES = src/c/parameters.c

libsciparameters_la_CFLAGS= \
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
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/string/includes

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciparameters.la
else
noinst_LTLIBRARIES = libsciparameters.la
endif


libsciparameters_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libsciparameters_la_SOURCES = $(PARAMETERS_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libsciparameters_la_SOURCES)
INCLUDE_FLAGS = $(libsciparameters_la_CFLAGS)

#### Target ######
modulename=parameters

#### parameters : Conf files ####
libparameters_la_rootdir = $(mydatadir)
libparameters_la_root_DATA =  license.txt 

#### parameters : init scripts ####
libparameters_la_etcdir = $(mydatadir)/etc
libparameters_la_etc_DATA = etc/parameters.quit etc/parameters.start

include $(top_srcdir)/Makefile.incl.am
