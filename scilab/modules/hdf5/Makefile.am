# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Allan CORNET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


#### Target ####
modulename=hdf5

#### hdf5 : Conf files ####
libscihdf5_la_rootdir = $(mydatadir)
libscihdf5_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### hdf5 : init scripts ####
libscihdf5_la_etcdir = $(mydatadir)/etc
libscihdf5_la_etc_DATA = etc/hdf5.quit etc/hdf5.start etc/hdf5_options.xml


libscihdf5_la_CFLAGS=  	-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/fileio/includes/

pkglib_LTLIBRARIES = libscihdf5-disable.la

if GUI
pkglib_LTLIBRARIES += libscihdf5.la
endif

libscihdf5_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) -lpthread $(XML_LIBS)


libscihdf5_la_LIBADD =  $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/dynamiclibrary/libscidynamiclibrary.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

#### hdf5 : gateway declaration ####
libscihdf5_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihdf5_la_sci_gateway_DATA = sci_gateway/hdf5_gateway.xml

#### hdf5 : include files ####
libscihdf5_la_includedir=$(pkgincludedir)/hdf5
libscihdf5_la_include_HEADERS = 

GATEWAY_C_SOURCES = 

HDF5_C_SOURCES = 

USEANT=1

libscihdf5_disable_la_SOURCES = src/nojvm/nojvm.c
libscihdf5_disable_la_CFLAGS = -I$(srcdir)/includes/ \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(srcdir)/src/c \
			-I$(top_srcdir)/modules/output_stream/includes/

libscihdf5_la_SOURCES = $(JVM_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscihdf5_la_SOURCES)
INCLUDE_FLAGS = $(libscihdf5_la_CFLAGS)


include $(top_srcdir)/Makefile.incl.am
