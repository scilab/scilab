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
libscitypes_la_root_DATA =  license.txt 


#### types : init scripts ####
libscitypes_la_etcdir = $(mydatadir)/etc
libscitypes_la_etc_DATA = etc/types.quit \
			  etc/types.start

include $(top_srcdir)/Makefile.incl.am


TYPES_JNI_SOURCES = 

TYPES_C_SOURCES = src/jni/getScilabVariable_wrap.c

TYPES_CXX_SOURCES = src/cpp/ScilabToJava.cpp \
	            src/jni/ScilabVariables.cpp \
	            src/jni/ScilabVariablesRefresh.cpp \
		    src/jni/ScilabPolynomialToJava.cpp

GIWS_WRAPPERS = src/jni/ScilabVariables.giws.xml

BUILT_SOURCES =
if GIWS
BUILT_SOURCES+=giws
endif

#GATEWAY_C_SOURCES =

#GATEWAY_CXX_SOURCES =

libscitypes_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			   -I$(srcdir)/includes/ \
			   -I$(srcdir)/src/jni/ \
			   -I$(srcdir)/src/cpp/ \
			   -I$(srcdir)/includes/ \
			   -I$(top_srcdir)/modules/commons/src/jni/ \
			   -I$(top_srcdir)/modules/jvm/includes/ \
			   -I$(top_srcdir)/modules/output_stream/includes/ \
			   -I$(top_srcdir)/modules/api_scilab/includes/ \
			   $(AM_CPPFLAGS)

TYPES_DISABLE_C_SOURCES =
libscitypes_disable_la_SOURCES = $(TYPES_DISABLE_C_SOURCES)
libscitypes_algo_la_SOURCES = $(TYPES_C_SOURCES) $(TYPES_JNI_SOURCES) $(TYPES_CXX_SOURCES)
libscitypes_algo_la_CPPFLAGS = $(libscitypes_la_CPPFLAGS)

libscitypes_la_LIBADD = $(top_builddir)/modules/commons/libscicommons.la \
			$(top_builddir)/modules/jvm/libscijvm.la

pkglib_LTLIBRARIES = libscitypes-disable.la

if GUI
pkglib_LTLIBRARIES += libscitypes.la
noinst_LTLIBRARIES = libscitypes-algo.la
endif


libscitypes_la_SOURCES = $(TYPES_C_SOURCES) $(TYPES_JNI_SOURCES) $(TYPES_CXX_SOURCES)
#libscitypes_algo_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)

# For the code check (splint)
CHECK_SRC= $(TYPES_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscitypes_la_CPPFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/sci_types.i \
		src/jni/getScilabVariable.i

if SWIG
BUILT_SOURCES+=swig
endif

#### types : gateway declaration ####
libscitypes_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitypes_la_sci_gateway_DATA =

if NEED_JAVA
USEANT=1
endif
