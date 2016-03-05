#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
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
modulename=history_browser


#### history_browser : Conf files ####
libscihistory_browser_la_rootdir = $(mydatadir)
libscihistory_browser_la_root_DATA =  license.txt


#### history_browser : init scripts ####
libscihistory_browser_la_etcdir = $(mydatadir)/etc
libscihistory_browser_la_etc_DATA = etc/history_browser.quit \
				etc/history_browser.start

include $(top_srcdir)/Makefile.incl.am

if GUI

# List of the JNI files
HISTORY_BROWSER_JNI_SOURCES = src/jni/CommandHistory.cpp

HISTORY_BROWSER_CPP_SOURCES = src/cpp/CommandHistory_Wrap.cpp

GIWS_WRAPPERS = src/jni/CommandHistory.giws.xml

HISTORY_BROWSER_C_SOURCES =

if GIWS
BUILT_SOURCES=giws
endif

else
HISTORY_BROWSER_C_SOURCES = src/nohistory_browser/CommandHistory_Wrap_Fake.c
endif

libscihistory_browser_la_CFLAGS=


libscihistory_browser_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/src/jni/ \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

# Without the history_browser module
libscihistory_browser_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
     $(AM_CPPFLAGS)

HISTORY_BROWSER_DISABLE_C_SOURCES =  src/nohistory_browser/CommandHistory_Wrap_Fake.c src/nohistory_browser/nohistory_browser.c
libscihistory_browser_disable_la_SOURCES = $(HISTORY_BROWSER_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscihistory_browser-disable.la

if GUI
pkglib_LTLIBRARIES += libscihistory_browser.la
endif

libscihistory_browser_la_LDFLAGS = $(AM_LDFLAGS)
libscihistory_browser_la_LIBADD = $(X_LIBS) $(X_EXTRA_LIBS)

libscihistory_browser_la_SOURCES = $(HISTORY_BROWSER_C_SOURCES) $(HISTORY_BROWSER_JNI_SOURCES) $(HISTORY_BROWSER_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(HISTORY_BROWSER_C_SOURCES)
INCLUDE_FLAGS = $(libscihistory_browser_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS =

if SWIG
BUILT_SOURCES=swig
endif


#### history_browser : gateway declaration ####
libscihistory_browser_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihistory_browser_la_sci_gateway_DATA =

if GUI
USEANT=1
endif
