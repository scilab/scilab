##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
##  Copyright (C) 2007-2008 - DIGITEO - Antoine ELIAS
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

pkglib_LTLIBRARIES 	= libsciabstractSyntaxTree.la

libsciabstractSyntaxTree_la_SOURCES 	=	src/cpp/libAst.cpp \
				src/cpp/visitor_common.cpp \
				src/cpp/printvisitor.cpp \
				src/cpp/debugvisitor.cpp \
				src/cpp/mutevisitor.cpp \
				src/cpp/shortcutvisitor.cpp

libsciabstractSyntaxTree_la_CXXFLAGS 	= -I$(top_srcdir)/modules/symbol/includes \
			-I $(top_srcdir)/modules/types/includes \
			-I $(top_srcdir)/modules/functions_manager/includes \
			-I $(top_srcdir)/modules/elementary_functions/includes \
			-I $(top_srcdir)/modules/operations/includes \
			-I $(top_srcdir)/modules/output_stream/includes \
			-I includes \
			-I src/cpp


libsciabstractSyntaxTree_la_includedir=$(pkgincludedir)/abstractSyntaxTree
libsciabstractSyntaxTree_la_include_HEADERS =

####################
## Automated test ##
####################
check_INCLUDES 		= -I$(top_srcdir)/modules/symbol/includes \
			-Iincludes/ \
			-Isrc/cpp

check_LDADD		= $(top_builddir)/modules/symbol/libsciSymbol.la \
			$(top_builddir)/modules/abstractSyntaxTree/libsciabstractSyntaxTree.la \
			$(top_builddir)/modules/types/libsciTypes.la

check_PROGRAMS 		= 	testConstExp \
											testVarExp \
											testControlExp

TESTS			= 	testConstExp \
							testVarExp \
							testControlExp

testConstExp_SOURCES 	= src/cpp/testConstExp.cpp
testConstExp_CXXFLAGS 	= $(check_INCLUDES)
testConstExp_LDADD	= $(check_LDADD)

testVarExp_SOURCES 	= src/cpp/testVarExp.cpp
testVarExp_CXXFLAGS 	= $(check_INCLUDES)
testVarExp_LDADD	= $(check_LDADD)

testControlExp_SOURCES 	= src/cpp/testControlExp.cpp
testControlExp_CXXFLAGS = $(check_INCLUDES)
testControlExp_LDADD	= $(check_LDADD)


