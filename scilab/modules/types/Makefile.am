#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# Copyright (C) DIGITEO - 2010 - Bruno JOFRET
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
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

#### types : include files ####
libscitypes_la_includedir=$(pkgincludedir)

TYPES_JAVA_C_SOURCES = src/jni/getScilabVariable_wrap.c

TYPES_JAVA_CPP_SOURCES = \
    src/cpp/ScilabToJava.cpp \
    src/jni/ScilabVariables.cpp \
    src/jni/ScilabVariablesRefresh.cpp \
    src/jni/ScilabPolynomialToJava.cpp

GIWS_WRAPPERS = src/jni/ScilabVariables.giws.xml

BUILT_SOURCES =
if GIWS
BUILT_SOURCES+=giws
endif

libscitypes_java_la_CFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(srcdir)/includes

libscitypes_java_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/src/jni \
    -I$(srcdir)/src/cpp \
    -I$(srcdir)/includes \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    $(AM_CPPFLAGS)


pkglib_LTLIBRARIES =
if GUI
pkglib_LTLIBRARIES += libscitypes-java.la
endif

libscitypes_algo_la_CPPFLAGS = 	\
    $(libscitypes_la_CPPFLAGS) \
    $(AM_CPPFLAGS)

libscitypes_java_la_LIBADD = \
			$(top_builddir)/modules/commons/libscicommons.la \
			$(top_builddir)/modules/jvm/libscijvm.la

libscitypes_java_la_SOURCES = $(TYPES_JAVA_C_SOURCES) $(TYPES_JAVA_CPP_SOURCES)

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

####################
## Automated test ##
####################
check_INCLUDES = \
    -Iincludes/ \
    -Isrc/cpp \
    -I$(top_srcdir)/modules/ast/includes

check_PROGRAMS 		= testTypes

TESTS			= testTypes

testTypes_SOURCES = src/cpp/test-types.cpp
testTypes_CPPFLAGS = $(check_INCLUDES)
testTypes_LDADD = $(top_builddir)/modules/ast/libsciast.la

