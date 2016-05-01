#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2007 - Sylvestre Ledru
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
modulename=renderer

#### renderer : Conf files ####
libscirenderer_la_rootdir = $(mydatadir)
libscirenderer_la_root_DATA =  license.txt

#### renderer : init scripts ####
libscirenderer_la_etcdir = $(mydatadir)/etc
libscirenderer_la_etc_DATA = etc/renderer.quit etc/renderer.start



# List of the c files
RENDERER_CXX_SOURCES = \
src/cpp/GetJavaProperty.cpp \
src/cpp/SetJavaProperty.cpp \
src/cpp/JavaInteraction.cpp \
src/cpp/RendererFontManager.cpp \
src/jni/XlFontManager.cpp \
src/jni/CallRenderer.cpp

# Includes need for the compilation
libscirenderer_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/jvm/includes/ \
				-I$(top_srcdir)/modules/commons/src/jni \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/graphic_export/includes/ \
				-I$(top_srcdir)/modules/elementary_functions/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/gui/includes/ \
				-I$(top_srcdir)/modules/string/includes/ \
				$(AM_CPPFLAGS)

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = $(RENDERER_CXX_SOURCES)

BUILT_SOURCES=

# For the code check (splint)
INCLUDE_FLAGS = $(libscirenderer_la_CPPFLAGS)

# needs the rendererer before $(top_builddir)/modules/jvm/libscijvm.la


GIWS_WRAPPERS= \
src/jni/XlFontManager.giws.xml \
src/jni/renderer.giws.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1

if GIWS
  BUILT_SOURCES+=giws
endif

if GUI
USEANT=1
pkglib_LTLIBRARIES = libscirenderer.la
endif


include $(top_srcdir)/Makefile.incl.am
