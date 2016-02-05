#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
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


#### Target ####
modulename=jvm

#### jvm : Conf files ####
libscijvm_la_rootdir = $(mydatadir)
libscijvm_la_root_DATA =  license.txt

#### jvm : init scripts ####
libscijvm_la_etcdir = $(mydatadir)/etc
libscijvm_la_etc_DATA = \
    etc/jvm.quit \
    etc/jvm.start




libscijvm_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/call_scilab/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/preferences/includes/ \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscijvm-disable.la

if GUI
USEANT=1
pkglib_LTLIBRARIES += libscijvm.la
noinst_LTLIBRARIES = libscijvm-algo.la
endif

libscijvm_la_LDFLAGS = -pthread $(AM_LDFLAGS)

# FIXME: This a platform specific flag, this should be done on configure.ac
if IS_MACOSX
libscijvm_la_LDFLAGS += -framework JavaVM
endif

libscijvm_la_LIBADD = libscijvm-algo.la $(XML_LIBS) $(JAVA_JNI_LIBS)

#### jvm : gateway declaration ####
libscijvm_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscijvm_la_sci_gateway_DATA = sci_gateway/jvm_gateway.xml

#### jvm :  dtd files ####
libscijvm_la_xmldir = $(mydatadir)/xml
libscijvm_la_xml_DATA = \
    xml/classpath.dtd  \
    xml/librarypath.dtd

#### jvm : include files ####
libscijvm_la_includedir=$(pkgincludedir)
libscijvm_la_include_HEADERS = \
    includes/getScilabJavaVM.h \
    includes/getScilabJNIEnv.h \
    includes/addToClasspath.h \
    includes/addToLibrarypath.h \
    includes/getScilabObject.h

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_javaclasspath.c \
    sci_gateway/c/sci_javalibrarypath.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_system_getproperty.cpp \
    sci_gateway/cpp/sci_system_setproperty.cpp \
    sci_gateway/cpp/jvm_gw.cpp

JVM_C_SOURCES = \
    src/c/InitializeJVM.c \
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

JVM_DISABLE_C_SOURCES = src/nojvm/nojvm.c
JVM_DISABLE_CPP_SOURCES = src/nojvm/nojvm_gw.cpp
libscijvm_disable_la_SOURCES = $(JVM_DISABLE_C_SOURCES) $(JVM_DISABLE_CPP_SOURCES)

libscijvm_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/functions_manager/includes/

libscijvm_algo_la_SOURCES = $(JVM_C_SOURCES)
libscijvm_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscijvm_algo_la_CPPFLAGS = $(libscijvm_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscijvm_la_SOURCES)
INCLUDE_FLAGS = $(libscijvm_la_CPPFLAGS)

CLEANFILES = etc/jvm_options.xml

include $(top_srcdir)/Makefile.incl.am
