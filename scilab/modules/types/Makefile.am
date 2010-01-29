##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

TYPES_CPP_SOURCES 	= src/cpp/types.cpp \
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
			src/cpp/struct.cpp

GATEWAY_CPP_SOURCES	= src/cpp/types_gw.cpp \
			sci_gateway/cpp/sci_list.cpp \
			sci_gateway/cpp/sci_struct.cpp

pkglib_LTLIBRARIES 	= libscitypes.la

libscitypes_la_SOURCES 	= $(TYPES_CPP_SOURCES) $(GATEWAY_CPP_SOURCES)

libscitypes_la_CXXFLAGS	=	-Iincludes/ \
				-I../elementary_functions/includes \
				-I../operations/includes \
				-I../symbol/includes \
				-I../abstractSyntaxTree/includes \
				-I../output_stream/includes \
				-I../parse/includes \
				-I../system_env/includes \
				-I ../functions_manager/includes \
				-I src/cpp

libscitypes_la_includedir=$(pkgincludedir)/Types

libscitypes_la_include_HEADERS =

####################
## Automated test ##
####################
check_INCLUDES 		= -Iincludes/ \
			-Isrc/cpp

check_PROGRAMS 		= testTypes

TESTS			= testTypes

testTypes_SOURCES 	= src/cpp/test-types.cpp
testTypes_CXXFLAGS 	= $(check_INCLUDES)
testTypes_LDADD	= $(top_builddir)/modules/types/libscitypes.la
