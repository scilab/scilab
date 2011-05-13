#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=graph


#### graph : Conf files ####
libscigraph_la_rootdir = $(mydatadir)
libscigraph_la_root_DATA =  license.txt version.xml


#### graph : init scripts ####
libscigraph_la_etcdir = $(mydatadir)/etc
libscigraph_la_etc_DATA = etc/graph.quit \
				etc/graph.start

include $(top_srcdir)/Makefile.incl.am


GRAPH_JNI_SOURCES = 

GRAPH_C_SOURCES = 

GRAPH_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = 

GATEWAY_CPP_SOURCES = 

libscigraph_la_CFLAGS= 


libscigraph_la_CPPFLAGS = 

# Without the graph module 
libscigraph_disable_la_CFLAGS = 

GRAPH_DISABLE_C_SOURCES = 
libscigraph_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscigraph_disable_la_SOURCES = $(GRAPH_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscigraph-disable.la

if GUI
pkglib_LTLIBRARIES += libscigraph.la 
endif

libscigraph_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscigraph_la_SOURCES = $(GRAPH_C_SOURCES) $(GRAPH_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(GRAPH_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GRAPH_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigraph_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
BUILT_SOURCES=swig
endif

libscigraph_la_LIBADD =  \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### graph : gateway declaration ####
libscigraph_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraph_la_sci_gateway_DATA = 

if GUI
USEANT=1
endif
