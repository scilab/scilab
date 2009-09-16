# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Sylvestre LEDRU
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

if GUI
#List of the c files
COMPLETION_JNI_SOURCES = src/jni/Completion_wrap.c 
USEANT=1

endif

# List of the c files
COMPLETION_C_SOURCES = src/c/completion.c \
src/c/completion_generic.c \
src/c/getfilesdictionary.c \
src/c/getfulldictionary.c \
src/c/getmacrosdictionary.c \
src/c/toolsdictionary.c \
src/c/completion_wrap_java.c \
src/c/getPartLine.c \
src/c/getCommonPart.c \
src/c/completeLine.c \
src/c/completeLine_wrap_java.c 

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/sci_completion.c \
sci_gateway/c/gw_completion.c

# Includes need for the compilation
libscicompletion_la_CFLAGS=  	$(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/fileio/includes/ \
				-I$(top_srcdir)/modules/functions/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscicompletion.la

libscicompletion_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

# All the sources needed by libscicompletion.la
libscicompletion_la_SOURCES = $(COMPLETION_JNI_SOURCES) $(COMPLETION_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(COMPLETION_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscicompletion_la_CFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libscicompletion_la_LIBADD =  $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 
#$(top_builddir)/modules/graphics/libscigraphics.la
#### Name of the module and the path ######
modulename=completion

#### completion : Conf files ####
libscicompletion_la_rootdir = $(mydatadir)
libscicompletion_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### completion : init scripts ####
libscicompletion_la_etcdir = $(mydatadir)/etc
libscicompletion_la_etc_DATA = etc/completion.quit etc/completion.start

#### completion : gateway declaration ####
libscicompletion_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicompletion_la_sci_gateway_DATA = sci_gateway/completion_gateway.xml

#### completion : include files ####
libscicompletion_la_includedir=$(pkgincludedir)
libscicompletion_la_include_HEADERS = includes/gw_completion.h \
includes/completion.h \
includes/getCommonPart.h \
includes/getPartLine.h \
includes/dynlib_completion.h \
includes/completeLine.h

#### completion : test file ####
libscicompletion_la_testutilitiesdir = $(mydatadir)/tests/utilities
libscicompletion_la_testutilities_DATA = tests/utilities/build_primitives.sce \
tests/utilities/sci_completeline.c \
tests/utilities/sci_getcommonpart.c \
tests/utilities/sci_getfilepartlevel.c \
tests/utilities/sci_getpartlevel.c

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/Completion.i 

if SWIG
BUILT_SOURCES=swig
endif


# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1


include $(top_srcdir)/Makefile.incl.am
