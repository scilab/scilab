#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#


#### Target ####
modulename=jvm

#### jvm : Conf files ####
libscijvm_la_rootdir = $(mydatadir)
libscijvm_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### jvm : init scripts ####
libscijvm_la_etcdir = $(mydatadir)/etc
libscijvm_la_etc_DATA = etc/jvm.quit etc/jvm.start etc/jvm_options.xml




libscijvm_la_CFLAGS=  	$(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/libs/dynamiclibrary/includes/ \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/c \
			$(XML_FLAGS) \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(top_srcdir)/modules/fileio/includes/ \
			-I$(top_srcdir)/modules/operations/includes/

pkglib_LTLIBRARIES = libscijvm-disable.la

if GUI
pkglib_LTLIBRARIES += libscijvm.la
endif

libscijvm_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) -lpthread $(XML_LIBS)

if IS_MACOSX
libscijvm_la_LDFLAGS += -framework JavaVM
endif

libscijvm_la_LIBADD =  $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/dynamiclibrary/libscidynamiclibrary.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

#### jvm : gateway declaration ####
libscijvm_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscijvm_la_sci_gateway_DATA = sci_gateway/jvm_gateway.xml

#### jvm : include files ####
libscijvm_la_includedir=$(pkgincludedir)/jvm
libscijvm_la_include_HEADERS = includes/TerminateJVM.h \
includes/getScilabJavaVM.h \
includes/getScilabJNIEnv.h \
includes/InitializeJVM.h \
includes/gw_jvm.h \
includes/addToClasspath.h \
includes/addToLibrarypath.h \
includes/catchIfJavaException.h \
includes/getScilabObject.h \
includes/loadBackGroundClassPath.h \
includes/loadOnUseClassPath.h

GATEWAY_C_SOURCES = sci_gateway/c/sci_system_getproperty.c \
sci_gateway/c/sci_system_setproperty.c \
sci_gateway/c/sci_with_embedded_jre.c \
sci_gateway/c/sci_javaclasspath.c \
sci_gateway/c/gw_jvm.c \
sci_gateway/c/sci_javalibrarypath.c


JVM_C_SOURCES = src/c/InitializeJVM.c \
src/c/TerminateJVM.c \
src/c/JVM_Unix.c \
src/c/JVM.c \
src/c/addToClasspath.c \
src/c/loadOnUseClassPath.c \
src/c/loadBackGroundClassPath.c \
src/c/getClasspath.c \
src/c/system_getproperty.c \
src/c/system_setproperty.c \
src/c/loadClasspath.c \
src/c/createMainScilabObject.c \
src/c/JVM_commons.c \
src/c/catchIfJavaException.c \
src/c/loadLibrarypath.c \
src/c/getLibrarypath.c \
src/c/addToLibrarypath.c \
src/c/getJvmOptions.c

USEANT=1

libscijvm_disable_la_SOURCES = src/nojvm/nojvm.c
libscijvm_disable_la_CFLAGS = -I$(srcdir)/includes/ \
			-I$(top_srcdir)/modules/call_scilab/includes \
			-I$(srcdir)/src/c \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/output_stream/includes/

libscijvm_la_SOURCES = $(JVM_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libscijvm_la_SOURCES)
INCLUDE_FLAGS = $(libscijvm_la_CFLAGS)


include $(top_srcdir)/Makefile.incl.am
