#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#


#### Name of the module and the path ######
modulename=graphic_export

#### graphic_export : Conf files ####
libscigraphic_export_la_rootdir = $(mydatadir)
libscigraphic_export_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### graphic_export : init scripts ####
libscigraphic_export_la_etcdir = $(mydatadir)/etc
libscigraphic_export_la_etc_DATA = etc/graphic_export.quit etc/graphic_export.start


# List of the c files
GRAPHIC_EXPORT_CPP_SOURCES = src/cpp/exportToFile.cpp \
src/jni/FileExporter.cpp \
src/cpp/GL2PSConstant.cpp \
src/cpp/GL2PSWrapping.cpp \
src/cpp/JoGLInterface.cpp  \
src/jni/GL2PSToJOGL.cpp \
src/jni/JOGLConstant.cpp \
src/jni/GiwsException.cpp

GRAPHIC_EXPORT_C_SOURCES = src/c/xs2file.c \
src/jni/GL2PSConstant_wrap.c \
src/jni/GL2PSWrapping_wrap.c \
src/c/gl2ps/gl2ps.c

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_graphic_export.c \
sci_gateway/c/sci_xs2bmp.c \
sci_gateway/c/sci_xs2gif.c \
sci_gateway/c/sci_xs2ppm.c \
sci_gateway/c/sci_xs2ps.c \
sci_gateway/c/sci_xs2svg.c \
sci_gateway/c/sci_xs2jpg.c \
sci_gateway/c/sci_xs2png.c \
sci_gateway/c/sci_xs2eps.c \
sci_gateway/c/sci_xs2pdf.c


# Includes need for the compilation
libscigraphic_export_la_CPPFLAGS =  	$(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/renderer/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/fileio/includes \
				-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/libs/doublylinkedlist/includes

# All the sources needed by libscigraphic_export.la
libscigraphic_export_la_SOURCES = $(GRAPHIC_EXPORT_C_SOURCES) $(GATEWAY_C_SOURCES) $(GRAPHIC_EXPORT_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GRAPHIC_EXPORT_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigraphic_export_la_CPPFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libscigraphic_export_la_LIBADD = $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

GIWS_WRAPPERS= src/jni/FileExporter.giws.xml \
src/jni/GL2PSToJOGL.giws.xml \
src/jni/JOGLConstant.giws.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1

USEANT=1

if GIWS
BUILT_SOURCES=giws
endif

# All the sources needed by libscigraphic_export.la
libscigraphic_export_disable_la_SOURCES = src/nographic_export/nographic_export.c

libscigraphic_export_disable_la_CPPFLAGS = -I$(top_srcdir)/modules/output_stream/includes

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscigraphic_export-disable.la

if GUI
pkglib_LTLIBRARIES += libscigraphic_export.la 
endif

libscigraphic_export_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

#### graphic_export : gateway declaration ####
libscigraphic_export_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraphic_export_la_sci_gateway_DATA = sci_gateway/graphic_export_gateway.xml

#### graphic_export : include files ####
libscigraphic_export_la_includedir=$(pkgincludedir)/graphic_export
libscigraphic_export_la_include_HEADERS = includes/gw_graphic_export.h \
includes/sci_xs2ppm.h \
includes/sci_xs2gif.h \
includes/xs2file.h \
includes/sci_xs2bmp.h \
includes/sci_xs2ps.h \
includes/sci_xs2eps.h \
includes/sci_xs2jpg.h \
includes/sci_xs2pdf.h \
includes/sci_xs2png.h \
includes/sci_xs2svg.h

include $(top_srcdir)/Makefile.incl.am
