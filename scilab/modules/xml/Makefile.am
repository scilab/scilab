#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO - Calixte DENIZET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=xml


#### xml : Conf files ####
libscixml_la_rootdir = $(mydatadir)
libscixml_la_root_DATA =  license.txt 


#### scinotes : init scripts & configuration files ####
libscixml_la_etcdir = $(mydatadir)/etc
libscixml_la_etc_DATA = etc/xml.quit \
				etc/xml.start

include $(top_srcdir)/Makefile.incl.am


XML_JNI_SOURCES = 

XML_C_SOURCES = 

XML_CPP_SOURCES = 


GATEWAY_C_SOURCES = sci_gateway/c/gw_xml.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_xmlRead.cpp \
sci_gateway/cpp/sci_xmlXPath.cpp

libscixml_la_CFLAGS= -I$(top_srcdir)/libs/MALLOC/includes/ \
    -I$(top_srcdir)/modules/core/includes

libscixml_la_CPPFLAGS = -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/api_scilab/includes

# Without the xml module 
libscixml_disable_la_CFLAGS = -Iincludes/

libscixml_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscixml_disable_la_SOURCES = $(XML_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscixml-disable.la

if GUI
    pkglib_LTLIBRARIES += libscixml.la 
    noinst_LTLIBRARIES = libscixml-algo.la 
endif

libscixml_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscixml_algo_la_SOURCES = $(XML_C_SOURCES) $(XML_JNI_SOURCES) $(XML_CPP_SOURCES)
libscixml_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) 
libscixml_algo_la_CFLAGS = $(libscixml_la_CFLAGS)
libscixml_algo_la_CPPFLAGS = $(libscixml_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(XML_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscixml_la_CFLAGS)


libscixml_la_LIBADD = libscixml-algo.la \
    $(top_builddir)/libs/MALLOC/libscimalloc.la


#### xml : gateway declaration ####
libscixml_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixml_la_sci_gateway_DATA = sci_gateway/xml_gateway.xml
