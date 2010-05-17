#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# Copyright (C) DIGITEO - 2010 - Bruno JOFRET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=types


#### types : Conf files ####
libscitypes_la_rootdir = $(mydatadir)
libscitypes_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### types : init scripts ####
libscitypes_la_etcdir = $(mydatadir)/etc
libscitypes_la_etc_DATA = etc/types.quit \
				etc/types.start

include $(top_srcdir)/Makefile.incl.am


TYPES_JNI_SOURCES = 

TYPES_C_SOURCES = 

TYPES_CPP_SOURCES = \
			src/cpp/types.cpp \
			src/cpp/int.cpp \
			src/cpp/string.cpp \
			src/cpp/double.cpp \
			src/cpp/bool.cpp \
			src/cpp/float.cpp \
			src/cpp/implicitlist.cpp \
			src/cpp/poly.cpp \
			src/cpp/matrixpoly.cpp \
			src/cpp/tostring_common.cpp \
			src/cpp/function.cpp \
			src/cpp/macrofile.cpp \
			src/cpp/macro.cpp \
			src/cpp/list.cpp \
			src/cpp/listdelete.cpp \
			src/cpp/listinsert.cpp \
			src/cpp/listoperation.cpp \
			src/cpp/listundefined.cpp \
			src/cpp/struct.cpp \
			src/cpp/cell.cpp

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = 

GATEWAY_CPP_SOURCES = sci_gateway/cpp/types_gw.cpp \
			sci_gateway/cpp/sci_list.cpp \
			sci_gateway/cpp/sci_struct.cpp \
			sci_gateway/cpp/sci_insert.cpp \
			sci_gateway/cpp/sci_null.cpp \
			sci_gateway/cpp/sci_cell.cpp \
			sci_gateway/cpp/sci_makecell.cpp

libscitypes_la_CFLAGS= 


libscitypes_la_CPPFLAGS = 	-Iincludes/ \
				-I../elementary_functions/includes \
				-I../operations/includes \
				-I../symbol/includes \
				-I../abstractSyntaxTree/includes \
				-I../output_stream/includes \
				-I../parse/includes \
				-I../system_env/includes \
				-I ../functions_manager/includes \
				-I src/cpp

pkglib_LTLIBRARIES = libscitypes.la 

libscitypes_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscitypes_la_SOURCES = $(TYPES_C_SOURCES) $(TYPES_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(TYPES_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(TYPES_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscitypes_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
BUILT_SOURCES=swig
endif

libscitypes_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### types : gateway declaration ####
libscitypes_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitypes_la_sci_gateway_DATA = 

if GUI
USEANT=1
endif

####################
## Automated test ##
####################
check_INCLUDES 		= -Iincludes/ \
			-Isrc/cpp

check_PROGRAMS 		= testTypes

TESTS			= testTypes

testTypes_SOURCES 	= src/cpp/test-types.cpp
testTypes_CPPFLAGS 	= $(check_INCLUDES)
testTypes_LDADD	= $(top_builddir)/modules/types/libscitypes.la

