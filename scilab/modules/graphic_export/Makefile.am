#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2008 - Sylvestre Ledru
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


#### Name of the module and the path ######
modulename=graphic_export

#### graphic_export : Conf files ####
libscigraphic_export_la_rootdir = $(mydatadir)
libscigraphic_export_la_root_DATA =  license.txt

#### graphic_export : init scripts ####
libscigraphic_export_la_etcdir = $(mydatadir)/etc
libscigraphic_export_la_etc_DATA = \
    etc/graphic_export.quit \
    etc/graphic_export.start


# List of the c files
GRAPHIC_EXPORT_CXX_SOURCES = \
    src/cpp/exportToFile.cpp \
    src/cpp/xs2file.cpp \
    src/jni/FileExporter.cpp \
    src/jni/Driver.cpp
    
# List of the gateway c files
GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_xs2bmp.c \
    sci_gateway/c/sci_xs2gif.c \
    sci_gateway/c/sci_xs2ppm.c \
    sci_gateway/c/sci_xs2ps.c \
    sci_gateway/c/sci_xs2svg.c \
    sci_gateway/c/sci_xs2jpg.c \
    sci_gateway/c/sci_xs2eps.c \
    sci_gateway/c/sci_xs2pdf.c \
    sci_gateway/c/sci_xs2png.c \
    sci_gateway/c/sci_xs2emf.c

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_driver.cpp \
    sci_gateway/cpp/sci_xinit.cpp \
    sci_gateway/cpp/sci_xend.cpp

# Includes need for the compilation
libscigraphic_export_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/jni/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/renderer/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni \
    -I$(top_srcdir)/modules/graphic_objects/includes/ \
    -I$(top_srcdir)/modules/gui/src/jni/ \
    -I$(top_srcdir)/modules/string/includes/ \
    $(JAVA_JNI_INCLUDE) \
    $(AM_CPPFLAGS)

# All the sources needed by libscigraphic_export.la
libscigraphic_export_algo_la_SOURCES = $(GRAPHIC_EXPORT_CXX_SOURCES)
libscigraphic_export_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscigraphic_export_algo_la_CPPFLAGS = $(libscigraphic_export_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
INCLUDE_FLAGS = $(libscigraphic_export_la_CPPFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libscigraphic_export_la_LIBADD = libscigraphic_export-algo.la

GIWS_WRAPPERS= src/jni/FileExporter.giws.xml \
src/jni/Driver.giws.xml

BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif

# All the sources needed by libscigraphic_export.la
libscigraphic_export_disable_la_SOURCES = src/nographic_export/nographic_export.c

libscigraphic_export_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
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
