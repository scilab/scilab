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
modulename=ui_data


#### ui_data : Conf files ####
libsciui_data_la_rootdir = $(mydatadir)
libsciui_data_la_root_DATA =  license.txt


#### ui_data : init scripts ####
libsciui_data_la_etcdir = $(mydatadir)/etc
libsciui_data_la_etc_DATA = \
	etc/newsfeed.css \
	etc/newsfeed.xml \
	etc/ui_data.quit \
	etc/ui_data.start \
	etc/variablebrowser_menubar.xml \
	etc/variableeditor_menubar.xml \
	etc/XConfiguration-variables.xml \
	etc/XConfiguration-variables.xsl

UI_DATA_JNI_SOURCES = \
    src/jni/BrowseVar.cpp \
    src/jni/EditVar.cpp \
	src/jni/FileBrowser.cpp

UI_DATA_C_SOURCES = \
    src/jni/ui_data_wrap.c \
    src/c/PutScilabVariable.c

UI_DATA_CXX_SOURCES = \
    src/cpp/BrowseVarManager.cpp \
    src/cpp/FileBrowserChDir.cpp

GIWS_WRAPPERS = \
    src/jni/BrowseVar.giws.xml \
    src/jni/EditVar.giws.xml \
    src/jni/FileBrowser.giws.xml

SWIG_WRAPPERS = \
    src/jni/ui_data.i

BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif
if SWIG
BUILT_SOURCES+=swig
endif


GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_editvar.cpp \
    sci_gateway/cpp/sci_browsevar.cpp \
    sci_gateway/cpp/sci_filebrowser.cpp \
    sci_gateway/cpp/sci_updatebrowsevar.cpp \
    sci_gateway/cpp/sci_closeEditvar.cpp \
    sci_gateway/cpp/ui_data_gw.cpp


libsciui_data_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    $(AM_CPPFLAGS)

# Without the ui_data module
libsciui_data_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    $(AM_CPPFLAGS)

UI_DATA_DISABLE_C_SOURCES = src/noui_data/noui_data.c
UI_DATA_DISABLE_CXX_SOURCES = src/noui_data/noui_data_gw.cpp
libsciui_data_disable_la_SOURCES = $(UI_DATA_DISABLE_C_SOURCES) $(UI_DATA_DISABLE_CXX_SOURCES)

pkglib_LTLIBRARIES = libsciui_data-disable.la

if GUI
pkglib_LTLIBRARIES += libsciui_data.la
noinst_LTLIBRARIES = libsciui_data-algo.la
endif

libsciui_data_la_LDFLAGS = $(AM_LDFLAGS)

libsciui_data_algo_la_SOURCES = $(UI_DATA_C_SOURCES) $(UI_DATA_JNI_SOURCES) $(UI_DATA_CXX_SOURCES)
libsciui_data_la_SOURCES = $(GATEWAY_CXX_SOURCES)
libsciui_data_algo_la_CPPFLAGS = $(libsciui_data_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(UI_DATA_C_SOURCES)
INCLUDE_FLAGS = $(libsciui_data_la_CPPFLAGS)

libsciui_data_la_LIBADD = libsciui_data-algo.la $(X_LIBS) $(X_EXTRA_LIBS)


#### ui_data : gateway declaration ####
libsciui_data_la_sci_gatewaydir = $(mydatadir)/sci_gateway

#### ui_data : icons files ####
#
# Generated with:
#    $ find images/ -type f -printf "%p \\\\\n" -name *.svg -or -name *.gif -or -name *.jpg -or -name *.png |sort -u
libsciui_data_la_iconsdir=$(mydatadir)/
nobase_libsciui_data_la_icons_DATA = \
images/icons/16x16/mimetypes/array_up.png \
images/icons/16x16/mimetypes/boolean.png \
images/icons/16x16/mimetypes/cell.png \
images/icons/16x16/mimetypes/double.png \
images/icons/16x16/mimetypes/fptr.png \
images/icons/16x16/mimetypes/function.png \
images/icons/16x16/mimetypes/handle.png \
images/icons/16x16/mimetypes/int.png \
images/icons/16x16/mimetypes/list.png \
images/icons/16x16/mimetypes/mlist.png \
images/icons/16x16/mimetypes/noicon.png \
images/icons/16x16/mimetypes/object.png \
images/icons/16x16/mimetypes/polynomial.png \
images/icons/16x16/mimetypes/sparse.png \
images/icons/16x16/mimetypes/string.png \
images/icons/16x16/mimetypes/timeseries.png \
images/icons/16x16/mimetypes/tlist.png \
images/icons/16x16/mimetypes/uint.png \
images/icons/16x16/mimetypes/user.png

if GUI
USEANT=1
endif

include $(top_srcdir)/Makefile.incl.am
