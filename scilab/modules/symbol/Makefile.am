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

SYMBOL_CXX_SOURCES 		= src/cpp/symbol.cpp \
				src/cpp/context.cpp \
				src/cpp/table.cpp

pkglib_LTLIBRARIES 		= libscisymbol.la

libscisymbol_la_SOURCES 	= $(SYMBOL_CXX_SOURCES)

libscisymbol_la_CPPFLAGS = \
	-I includes/ \
	-I src/cpp \
    -I$(top_srcdir)/modules/dynamic_link/includes \
	-I$(top_builddir)/modules/types/includes \
	-I$(top_builddir)/modules/functions_manager/includes \
	-I$(top_builddir)/modules/elementary_functions/includes \
	-I$(top_builddir)/modules/operations/includes \
	-I$(top_builddir)/modules/ast/includes \
	-I$(top_builddir)/modules/localization/includes \
	$(AM_CPPFLAGS)


libscisymbol_la_LIBADD =

libscisymbol_la_includedir=$(pkgincludedir)/Symbol

libscisymbol_la_include_HEADERS = includes/symbol.hxx

# For the code check (splint)
INCLUDE_FLAGS = $(libscisymbol_la_CPPFLAGS)

####################
## Automated test ##
####################
check_INCLUDES 		= -Iincludes/ \
			-I $(top_builddir)/modules/types/includes \
			-Isrc/cpp

check_LDADD		= $(top_builddir)/modules/symbol/libscisymbol.la \
			$(top_builddir)/modules/types/libsciTypes.la

check_PROGRAMS 		= 	testSymbol \
				testScope \
				testStack \
				testHeap

TESTS			= 	testSymbol \
				testScope \
				testStack \
				testHeap

testSymbol_SOURCES 	= src/cpp/test-symbol.cpp
testSymbol_CXXFLAGS 	= $(check_INCLUDES)
testSymbol_LDADD	= $(check_LDADD)

testScope_SOURCES 	= src/cpp/test-scope.cpp
testScope_CXXFLAGS 	= $(check_INCLUDES)
testScope_LDADD		= $(check_LDADD)

testStack_SOURCES 	= src/cpp/test-stack.cpp
testStack_CXXFLAGS 	= $(check_INCLUDES)
testStack_LDADD		= $(check_LDADD)

testHeap_SOURCES 	= src/cpp/test-heap.cpp
testHeap_CXXFLAGS 	= $(check_INCLUDES)
testHeap_LDADD		= $(check_LDADD)

include $(top_srcdir)/Makefile.incl.am
