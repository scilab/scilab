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
libscitypes_la_root_DATA =  license.txt


#### types : init scripts ####
libscitypes_la_etcdir = $(mydatadir)/etc
libscitypes_la_etc_DATA = etc/types.quit \
				etc/types.start

include $(top_srcdir)/Makefile.incl.am


TYPES_JNI_SOURCES =

TYPES_C_SOURCES =

TYPES_CPP_SOURCES = \
			src/cpp/types.cpp \
			src/cpp/string.cpp \
			src/cpp/double.cpp \
			src/cpp/bool.cpp \
			src/cpp/float.cpp \
			src/cpp/implicitlist.cpp \
			src/cpp/polynom.cpp \
			src/cpp/singlepoly.cpp \
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
			src/cpp/singlestruct.cpp \
			src/cpp/cell.cpp \
			src/cpp/tlist.cpp \
			src/cpp/file.cpp \
			src/cpp/mlist.cpp \
			src/cpp/threadId.cpp \
			src/cpp/arrayof.cpp \
			src/cpp/int8.cpp \
			src/cpp/uint8.cpp \
			src/cpp/int16.cpp \
			src/cpp/uint16.cpp \
			src/cpp/int32.cpp \
			src/cpp/uint32.cpp \
			src/cpp/int64.cpp \
			src/cpp/uint64.cpp \
            src/cpp/types_tools.cpp

GIWS_WRAPPERS =

if GIWS
BUILT_SOURCES=giws
endif


#GATEWAY_C_SOURCES =

GATEWAY_CPP_SOURCES = sci_gateway/cpp/types_gw.cpp \
			sci_gateway/cpp/sci_list.cpp \
			sci_gateway/cpp/sci_struct.cpp \
			sci_gateway/cpp/sci_insert.cpp \
			sci_gateway/cpp/sci_null.cpp \
			sci_gateway/cpp/sci_cell.cpp \
			sci_gateway/cpp/sci_makecell.cpp \
			sci_gateway/cpp/sci_usertype.cpp \
			sci_gateway/cpp/sci_typeof.cpp \
			sci_gateway/cpp/sci_tlist.cpp \
			sci_gateway/cpp/sci_mlist.cpp \
			sci_gateway/cpp/sci_isfield.cpp \
			sci_gateway/cpp/sci_fieldnames.cpp \
			sci_gateway/cpp/sci_getfield.cpp

libscitypes_la_CFLAGS=

libscitypes_la_CPPFLAGS = 	-Iincludes/ \
				-I$(top_srcdir)/modules/elementary_functions/includes \
				-I$(top_srcdir)/modules/operations/includes \
				-I$(top_srcdir)/modules/symbol/includes \
				-I$(top_srcdir)/modules/abstractSyntaxTree/includes \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/parse/includes \
				-I$(top_srcdir)/modules/system_env/includes \
				-I$(top_srcdir)/modules/functions_manager/includes \
				-I$(top_srcdir)/modules/string/includes \
				-I$(top_srcdir)/modules/threads/includes \
				-I src/cpp

libscitypes_algo_la_CPPFLAGS = $(libscitypes_la_CPPFLAGS)

# Without the types module
libscitypes_disable_la_CFLAGS =

TYPES_DISABLE_C_SOURCES =
libscitypes_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscitypes_disable_la_SOURCES = $(TYPES_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscitypes-disable.la

if GUI
pkglib_LTLIBRARIES += libscitypes.la
noinst_LTLIBRARIES = libscitypes-algo.la
endif

libscitypes_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscitypes_la_SOURCES = $(TYPES_C_SOURCES) $(TYPES_JNI_SOURCES) $(TYPES_CPP_SOURCES)
libscitypes_algo_la_SOURCES = $(GATEWAY_CPP_SOURCES) #$(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(TYPES_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscitypes_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/sci_types.i


if SWIG
BUILT_SOURCES=swig
endif

libscitypes_la_LIBADD =  \
libscitypes-algo.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### types : gateway declaration ####
libscitypes_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscitypes_la_sci_gateway_DATA =

if NEED_JAVA
USEANT=1
endif

####################
## Automated test ##
####################
check_INCLUDES 		= -Iincludes/ \
			-Isrc/cpp \
			-I$(top_srcdir)/modules/abstractSyntaxTree/includes

check_PROGRAMS 		= testTypes

TESTS			= testTypes

testTypes_SOURCES 	= src/cpp/test-types.cpp
testTypes_CPPFLAGS 	= $(check_INCLUDES)
testTypes_LDADD	= 	$(top_builddir)/modules/types/libscitypes-algo.la \
					$(top_builddir)/modules/abstractSyntaxTree/libsciabstractSyntaxTree.la

