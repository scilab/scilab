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
modulename=commons


#### commons : Conf files ####
libscicommons_la_rootdir = $(mydatadir)
libscicommons_la_root_DATA =  license.txt version.xml


#### commons : init scripts ####
libscicommons_la_etcdir = $(mydatadir)/etc
libscicommons_la_etc_DATA = etc/commons.quit \
				etc/commons.start

include $(top_srcdir)/Makefile.incl.am


COMMONS_JNI_SOURCES = 

COMMONS_C_SOURCES =  src/jni/ScilabCommons_wrap.c

COMMONS_CPP_SOURCES =

COMMONS_FORTRAN_SOURCES = src/fortran/empty.f

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = 

GATEWAY_CPP_SOURCES = 

libscicommons_la_CFLAGS= $(JAVA_JNI_INCLUDE)


libscicommons_la_CPPFLAGS = 

# Without the commons module 
libscicommons_disable_la_CFLAGS = 

COMMONS_DISABLE_C_SOURCES = 
libscicommons_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscicommons_disable_la_SOURCES = $(COMMONS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscicommons-disable.la

if GUI
pkglib_LTLIBRARIES += libscicommons.la 
endif

libscicommons_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscicommons_la_SOURCES = $(COMMONS_C_SOURCES) $(COMMONS_FORTRAN_SOURCES) $(COMMONS_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(COMMONS_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(COMMONS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscicommons_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/ScilabCommons.i

if SWIG
BUILT_SOURCES=swig
endif

libscicommons_la_LIBADD = $(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### commons : gateway declaration ####
libscicommons_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicommons_la_sci_gateway_DATA = 

if GUI
USEANT=1
endif
