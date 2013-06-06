#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=external_objects_java


#### external_objects_java : Conf files ####
libsciexternal_objects_java_la_rootdir = $(mydatadir)
libsciexternal_objects_java_la_root_DATA =  license.txt


#### external_objects_java : init scripts & configuration files ####
libsciexternal_objects_java_la_etcdir = $(mydatadir)/etc
libsciexternal_objects_java_la_etc_DATA = etc/external_objects_java.quit \
									 etc/external_objects_java.start

include $(top_srcdir)/Makefile.incl.am

EXTERNAL_OBJECTS_JAVA_JNI_SOURCES = 

EXTERNAL_OBJECTS_JAVA_C_SOURCES = 

EXTERNAL_OBJECTS_JAVA_CPP_SOURCES = src/cpp/ScilabJavaEnvironment.cpp \
src/cpp/NoMoreScilabMemoryException.cpp \
src/cpp/JavaOptionsSetter.cpp \
src/cpp/ScilabJavaEnvironmentWrapper.cpp \
src/jni/ScilabJavaArray.cpp \
src/jni/ScilabJavaCompiler.cpp \
src/jni/ScilabJavaClass.cpp \
src/jni/ScilabJavaObject.cpp \
src/jni/ScilabClassLoader.cpp

#src/cpp/ScilabObjectsCPP.cpp
# src/cpp/WrapAsDirectBuffer.cpp
GATEWAY_C_SOURCES = sci_gateway/c/gw_external_objects_java.c \
sci_gateway/cpp/sci_jimport.cpp \
sci_gateway/cpp/sci_jgetclassname.cpp \
sci_gateway/cpp/sci_jwrap.cpp \
sci_gateway/cpp/sci_jgetmethods.cpp \
sci_gateway/cpp/sci_jgetfields.cpp \
sci_gateway/cpp/sci_jgetfield.cpp \
sci_gateway/cpp/sci_jcompile.cpp \
sci_gateway/cpp/sci_jnewInstance.cpp \
sci_gateway/cpp/sci_jremove.cpp \
sci_gateway/cpp/sci_jexists.cpp \
sci_gateway/cpp/sci_junwrap.cpp \
sci_gateway/cpp/sci_junwraprem.cpp \
sci_gateway/cpp/sci_jenableTrace.cpp \
sci_gateway/cpp/sci_jdisableTrace.cpp \
sci_gateway/cpp/sci_jdeff.cpp \
sci_gateway/cpp/sci_jvoid.cpp \
sci_gateway/cpp/sci_jarray.cpp \
sci_gateway/cpp/sci_jcast.cpp \
sci_gateway/cpp/sci_jallowClassReloading.cpp

GIWS_WRAPPERS = src/jni/ScilabObjects.giws.xml

if GIWS
BUILT_SOURCES=giws
endif

GATEWAY_CPP_SOURCES =
libsciexternal_objects_java_la_CFLAGS= -I$(srcdir)/includes/ \
             -I$(top_srcdir)/libs/MALLOC/includes/ \
             -I$(top_srcdir)/modules/core/includes \
             -I$(top_srcdir)/modules/api_scilab/includes \
             -I$(top_srcdir)/modules/output_stream/includes \
             -I$(top_srcdir)/modules/string/includes \
             -I$(top_srcdir)/modules/fileio/includes


libsciexternal_objects_java_la_CPPFLAGS = -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/libs/MALLOC/includes/ \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/jvm/includes \
    -I$(top_srcdir)/modules/core/src/c \
    -I$(top_srcdir)/modules/external_objects/includes \
	-I$(top_srcdir)/modules/commons/src/jni/ \
    $(JAVA_JNI_INCLUDE)


pkglib_LTLIBRARIES = libsciexternal_objects_java.la 
noinst_LTLIBRARIES = libsciexternal_objects_java-algo.la 

libsciexternal_objects_java_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libsciexternal_objects_java_algo_la_SOURCES = $(EXTERNAL_OBJECTS_JAVA_C_SOURCES) $(EXTERNAL_OBJECTS_JAVA_JNI_SOURCES) $(EXTERNAL_OBJECTS_JAVA_CPP_SOURCES)
libsciexternal_objects_java_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) 
libsciexternal_objects_java_algo_la_CFLAGS = $(libsciexternal_objects_java_la_CFLAGS)
libsciexternal_objects_java_algo_la_CPPFLAGS = $(libsciexternal_objects_java_la_CPPFLAGS)

libsciexternal_objects_java_la_LIBADD = libsciexternal_objects_java-algo.la $(top_builddir)/modules/commons/libscicommons.la $(top_builddir)/modules/jvm/libscijvm.la

# For the code check (splint)
CHECK_SRC= $(EXTERNAL_OBJECTS_JAVA_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciexternal_objects_java_la_CFLAGS)

#### external_objects_java : gateway declaration ####
libsciexternal_objects_java_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciexternal_objects_java_la_sci_gateway_DATA = sci_gateway/external_objects_java_gateway.xml

#### external_objects_java : include files ####
libsciexternal_objects_java_la_includedir=$(pkgincludedir)
libsciexternal_objects_java_la_include_HEADERS =

if GUI
USEANT=1
endif
