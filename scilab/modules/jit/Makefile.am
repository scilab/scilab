##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2013 - S/E - Sylvestre Ledru
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

pkglib_LTLIBRARIES 	= libscijit.la

JIT_CPP_SOURCES = \
	src/cpp/jitter.cpp

GATEWAY_CPP_SOURCES =   \
	sci_gateway/cpp/jit_gw.cpp \
    sci_gateway/cpp/sci_enableJIT.cpp \
    sci_gateway/cpp/sci_disableJIT.cpp \
    sci_gateway/cpp/sci_isJIT.cpp

libscijit_la_CPPFLAGS = \
	-Iincludes \
	-Isrc/cpp \
    -I$(top_srcdir)/modules/types/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/symbol/includes \
    -I$(top_srcdir)/modules/operations/includes \
    -I$(top_srcdir)/modules/system_env/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/console/includes \
    -I$(top_srcdir)/modules/ast/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/functions_manager/includes \
    $(EIGEN_CPPFLAGS) \
	$(VMKIT_CPPFLAGS) \
	$(AM_CPPFLAGS)

libscijit_la_SOURCES = $(JIT_CPP_SOURCES) $(GATEWAY_CPP_SOURCES)

libscijit_la_includedir=$(pkgincludedir)
libscijit_la_include_HEADERS =

####################
## Automated test ##
####################
check_INCLUDES = \
    -I$(top_srcdir)/modules/symbol/includes \
	-Iincludes/ \
	-Isrc/cpp

check_LDADD	= \
    $(top_builddir)/modules/symbol/libsciSymbol.la \
	$(top_builddir)/modules/jit/libscijit.la \
	$(top_builddir)/modules/types/libsciTypes.la

testConstExp_SOURCES 	= src/cpp/testConstExp.cpp
testConstExp_CXXFLAGS 	= $(check_INCLUDES)
testConstExp_LDADD	= $(check_LDADD)

testVarExp_SOURCES 	= src/cpp/testVarExp.cpp
testVarExp_CXXFLAGS 	= $(check_INCLUDES)
testVarExp_LDADD	= $(check_LDADD)

testControlExp_SOURCES 	= src/cpp/testControlExp.cpp
testControlExp_CXXFLAGS = $(check_INCLUDES)
testControlExp_LDADD	= $(check_LDADD)

# This target transforms a basic code to llvm + clang code for the JIT
BUILT_SOURCES = src/cpp/llvm-wrapper-generated.cpp
src/cpp/llvm-wrapper-generated.cpp: src/cpp/llvm-wrapper.cpp
	clang++ -emit-llvm  -I$(top_srcdir)/modules/types/includes/ -I$(top_srcdir)/modules/operations/includes/ -I$(top_srcdir)/modules/core/includes/  -I$(top_srcdir)/modules/ast/includes/ -I$(top_srcdir)/modules/localization/includes/ -I$(top_srcdir)/modules/symbol/includes/ -I$(top_srcdir)/modules/dynamic_link/includes/ $(EIGEN_CPPFLAGS) -c src/cpp/llvm-wrapper.cpp -o - |  llc-3.3 -march=cpp -cppgen=contents -o src/cpp/llvm-wrapper-generated.cpp || exit 1

include $(top_srcdir)/Makefile.incl.am

