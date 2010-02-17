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

FUNCTION_MANAGER_CPP_SOURCES 	= src/cpp/funcmanager.cpp \
				$(top_builddir)/modules/system_env/src/cpp/configvariable.cpp


pkglib_LTLIBRARIES 	= libscifunctions_manager.la

libscifunctions_manager_la_SOURCES 	= $(FUNCTION_MANAGER_CPP_SOURCES)

libscifunctions_manager_la_CXXFLAGS	=	-I$(top_builddir)/modules/types/includes/ \
						-I$(top_builddir)/modules/core/includes/ \
						-I$(top_builddir)/modules/string/includes/ \
						-I$(top_builddir)/modules/symbol/includes/ \
						-I$(top_builddir)/modules/elementary_functions/includes/ \
						-I$(top_builddir)/modules/operations/includes/ \
						-I$(top_builddir)/modules/system_env/includes/ \
						-I$(top_builddir)/modules/api_scilab/includes/ \
						-I$(top_builddir)/modules/fileio/includes/ \
						-I$(top_builddir)/modules/abstractSyntaxTree/includes/ \
						-I$(top_builddir)/modules/parse/includes/ \
						-Iincludes/ \
						-Isrc/cpp \
						$(XML_FLAGS)

libscifunctions_manager_la_LIBADD = $(top_builddir)/modules/elementary_functions/libscielementary_functions.la

libscifunctions_manager_la_includedir=$(pkgincludedir)/Types

libscifunctions_manager_la_include_HEADERS =

####################
## Automated test ##
####################
check_INCLUDES 		=	-I$(top_builddir)/modules/types/includes/ \
				-Iincludes/ \
				-Isrc/cpp

check_PROGRAMS 		= testFunctionManager

TESTS			= testFunctionManager

testFunctionManager_SOURCES 	=	src/cpp/test-function.cpp \
					src/cpp/foo.cpp \
					src/cpp/cosinus.cpp
testFunctionManager_CXXFLAGS 	= $(check_INCLUDES)
testFunctionManager_LDADD	=	$(top_builddir)/modules/types/libsciTypes.la \
					$(top_builddir)/modules/functions_manager/libscifunctions_manager.la
