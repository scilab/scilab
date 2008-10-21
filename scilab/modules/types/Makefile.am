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
			src/cpp/uint.cpp \
			src/cpp/implicitlist.cpp

pkglib_LTLIBRARIES 	= libsciTypes.la

libsciTypes_la_SOURCES 	= $(TYPES_CPP_SOURCES)

libsciTypes_la_CXXFLAGS	=	-Iincludes/ \
				-Isrc/cpp

libsciTypes_la_includedir=$(pkgincludedir)/Types

libsciTypes_la_include_HEADERS =

####################
## Automated test ##
####################
check_INCLUDES 		= -Iincludes/ \
			-Isrc/cpp

check_PROGRAMS 		= testTypes

TESTS			= testTypes

testTypes_SOURCES 	= src/cpp/test-types.cpp
testTypes_CXXFLAGS 	= $(check_INCLUDES)
testTypes_LDADD	= $(top_builddir)/modules/types/libsciTypes.la
