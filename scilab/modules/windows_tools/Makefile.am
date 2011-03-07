# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

# List of the c files
WINDOWS_TOOLS_C_SOURCES = src/nowindows_tools/nowindows_tools.c

# Includes need for the compilation
libsciwindows_tools_la_CFLAGS=  -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libsciwindows_tools.la

libsciwindows_tools_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

# All the sources needed by libsciwindows_tools.la
libsciwindows_tools_la_SOURCES = $(WINDOWS_TOOLS_C_SOURCES)

## Libraries necessary
## Change to include other Scilab modules/lib
libsciwindows_tools_la_LIBADD =  $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Name of the module and the path ######
modulename=windows_tools

#### windows_tools : Conf files ####
libsciwindows_tools_la_rootdir = $(mydatadir)
libsciwindows_tools_la_root_DATA =  license.txt version.xml

#### windows_tools : init scripts ####
libsciwindows_tools_la_etcdir = $(mydatadir)/etc
libsciwindows_tools_la_etc_DATA = etc/windows_tools.quit etc/windows_tools.start

#### windows_tools : gateway declaration ####
libsciwindows_tools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciwindows_tools_la_sci_gateway_DATA = sci_gateway/windows_tools_gateway.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
