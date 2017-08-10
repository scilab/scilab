# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006-2008 - INRIA - Sylvestre LEDRU
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.


LOCALIZATION_C_SOURCES = \
    src/c/InitializeLocalization.c \
    src/c/setgetlanguage.c \
    src/c/getLcidString.c \
    src/c/charEncoding.c \
    src/c/setdefaultlanguage.c

if NEED_JAVA
LOCALIZATION_C_SOURCES += \
    src/jni/Messages_wrap.c \
    src/jni/LocaleToLCID_wrap.c \
    src/jni/setdefaultlanguage_wrap.c
endif

if IS_MACOSX
LOCALIZATION_C_SOURCES += \
    src/c/getLocaleInfo_Apple.c
endif

LOCALIZATION_CPP_SOURCES = \
    src/cpp/localization.cpp \
    src/cpp/UTF8.cpp

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_addlocalizationdomain.c \
    sci_gateway/c/sci_gettext.c

GATEWAY_CPP_SOURCES = \
	sci_gateway/cpp/localization_gw.cpp \
	sci_gateway/cpp/sci_setdefaultlanguage.cpp \
	sci_gateway/cpp/sci_getdefaultlanguage.cpp \
	sci_gateway/cpp/sci_setlanguage.cpp \
	sci_gateway/cpp/sci_getlanguage.cpp

libscilocalization_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscilocalization.la
noinst_LTLIBRARIES = libscilocalization-algo.la

libscilocalization_la_LDFLAGS = $(AM_LDFLAGS)
libscilocalization_la_CFLAGS = $(AM_CFLAGS)

libscilocalization_algo_la_SOURCES = $(LOCALIZATION_C_SOURCES) $(LOCALIZATION_CPP_SOURCES)
libscilocalization_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscilocalization_algo_la_CPPFLAGS = $(libscilocalization_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(libscilocalization_la_SOURCES)
INCLUDE_FLAGS = $(libscilocalization_la_CPPFLAGS)

libscilocalization_la_LIBADD = libscilocalization-algo.la

# FIXME: platform dependent things should be in configure.ac
if IS_MACOSX
libscilocalization_la_LDFLAGS += -framework Cocoa
libscilocalization_la_LIBADD += -lintl
libscilocalization_la_CFLAGS += -framework Cocoa
endif

# $(top_builddir)/modules/io/libsciio.la
#### Target ######
modulename=localization

#### localization : Conf files ####
libscilocalization_la_rootdir = $(mydatadir)
libscilocalization_la_root_DATA =  license.txt


####localization : init scripts ####
libscilocalization_la_etcdir = $(mydatadir)/etc
libscilocalization_la_etc_DATA = etc/localization.quit etc/localization.start

####�localization : gateway declaration ####
libscilocalization_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscilocalization_la_sci_gateway_DATA = sci_gateway/localization_gateway.xml


####localization : include files ####
libscilocalization_la_includedir=$(pkgincludedir)
libscilocalization_la_include_HEADERS = includes/localization.h \
includes/localization.hxx \
includes/dynlib_localization.h \
includes/setgetlanguage.h \
includes/charEncoding.h \
includes/UTF8.hxx

if GUI
USEANT=1
endif

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/Messages.i \
src/jni/LocaleToLCID.i

if SWIG
BUILT_SOURCES=swig
endif


include $(top_srcdir)/Makefile.incl.am
