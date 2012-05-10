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
libscigraphic_export_la_root_DATA =  license.txt 

#### graphic_export : init scripts ####
libscigraphic_export_la_etcdir = $(mydatadir)/etc
libscigraphic_export_la_etc_DATA = etc/graphic_export.quit etc/graphic_export.start


# List of the c files
GRAPHIC_EXPORT_CXX_SOURCES = src/cpp/exportToFile.cpp \
src/jni/FileExporter.cpp \
src/jni/Driver.cpp \
src/jni/JOGLConstant.cpp

GRAPHIC_EXPORT_C_SOURCES = src/c/xs2file.c

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_graphic_export.c \
sci_gateway/c/sci_xs2bmp.c \
sci_gateway/c/sci_xs2gif.c \
sci_gateway/c/sci_xs2ppm.c \
sci_gateway/c/sci_xs2ps.c \
sci_gateway/c/sci_xs2svg.c \
sci_gateway/c/sci_xs2jpg.c \
sci_gateway/c/sci_xs2eps.c \
sci_gateway/c/sci_xs2pdf.c \
sci_gateway/c/sci_xs2png.c

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_driver.cpp \
sci_gateway/cpp/sci_xinit.cpp \
sci_gateway/cpp/sci_xend.cpp

# Includes need for the compilation
libscigraphic_export_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/jvm/includes/ \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/renderer/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/fileio/includes/ \
				-I$(top_srcdir)/modules/commons/src/jni \
				-I$(top_srcdir)/libs/doublylinkedlist/includes/ \
				-I$(top_srcdir)/modules/graphic_objects/includes/ \
				-I$(top_srcdir)/modules/gui/src/jni/ \
				$(AM_CPPFLAGS)

# All the sources needed by libscigraphic_export.la
libscigraphic_export_algo_la_SOURCES = $(GRAPHIC_EXPORT_C_SOURCES) $(GRAPHIC_EXPORT_CXX_SOURCES)
libscigraphic_export_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscigraphic_export_algo_la_CPPFLAGS = $(libscigraphic_export_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(GRAPHIC_EXPORT_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
INCLUDE_FLAGS = $(libscigraphic_export_la_CPPFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libscigraphic_export_la_LIBADD = libscigraphic_export-algo.la

GIWS_WRAPPERS= src/jni/FileExporter.giws.xml \
src/jni/JOGLConstant.giws.xml \
src/jni/Driver.giws.xml

BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif

# All the sources needed by libscigraphic_export.la
libscigraphic_export_disable_la_SOURCES = src/nographic_export/nographic_export.c

libscigraphic_export_disable_la_CPPFLAGS = -I$(srcdir)/includes/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/localization/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	$(AM_CPPFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscigraphic_export-disable.la

if GUI
USEANT=1
pkglib_LTLIBRARIES += libscigraphic_export.la 
noinst_LTLIBRARIES = libscigraphic_export-algo.la 
endif


#### graphic_export : gateway declaration ####
libscigraphic_export_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraphic_export_la_sci_gateway_DATA = sci_gateway/graphic_export_gateway.xml

include $(top_srcdir)/Makefile.incl.am
