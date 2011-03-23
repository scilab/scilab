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
modulename=types


#### types : Conf files ####
libscitypes_la_rootdir = $(mydatadir)
libscitypes_la_root_DATA =  license.txt version.xml


#### types : init scripts ####
libscitypes_la_etcdir = $(mydatadir)/etc
libscitypes_la_etc_DATA = etc/types.quit \
				etc/types.start

include $(top_srcdir)/Makefile.incl.am


TYPES_JNI_SOURCES = 

TYPES_C_SOURCES = 

TYPES_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = 

GATEWAY_CPP_SOURCES = 

libscitypes_la_CFLAGS= 


libscitypes_la_CPPFLAGS = 

# Without the types module 
libscitypes_disable_la_CFLAGS = 

TYPES_DISABLE_C_SOURCES = 
libscitypes_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscitypes_disable_la_SOURCES = $(TYPES_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscitypes-disable.la

if GUI
pkglib_LTLIBRARIES += libscitypes.la 
endif

libscitypes_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscitypes_la_SOURCES = $(TYPES_C_SOURCES) $(TYPES_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(TYPES_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(TYPES_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscitypes_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/sci_types.i



if SWIG
BUILT_SOURCES=swig
endif

libscitypes_la_LIBADD =  \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### types : gateway declaration ####
libscitypes_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitypes_la_sci_gateway_DATA = 

if NEED_JAVA
USEANT=1
endif
