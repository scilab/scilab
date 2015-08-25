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

#### Target ######
modulename=slint

####io : init scripts ####
libscislint_la_etcdir = $(mydatadir)/etc
libscislint_la_etc_DATA = \
    etc/slint.quit \
    etc/slint.start

include $(top_srcdir)/Makefile.incl.am

SLINT_CXX_SOURCES = \
src/cpp/SLintContext.cpp \
src/cpp/SLintChecker.cpp \
src/cpp/SLintVisitor.cpp \
src/cpp/SLint.cpp \
src/cpp/SLintOptions.cpp \
src/cpp/SciFile.cpp \
src/cpp/FileException.cpp \
src/cpp/PCREMatcher.cpp \
src/cpp/PCREException.cpp \
src/cpp/XMLConfig.cpp \
src/cpp/SLintXMLException.cpp \
src/cpp/SLintScilabResult.cpp \
src/cpp/FunctionNameChecker.cpp \
src/cpp/FunctionArgsChecker.cpp \
src/cpp/VariableNameChecker.cpp \
src/cpp/VariablesChecker.cpp \
src/cpp/UselessArgChecker.cpp \
src/cpp/UselessRetChecker.cpp \
src/cpp/RedefinitionChecker.cpp \
src/cpp/SingleInstrChecker.cpp \
src/cpp/EmptyBlockChecker.cpp \
src/cpp/SemicolonAtEOLChecker.cpp \
src/cpp/MopenMcloseChecker.cpp \
src/cpp/PrintfChecker.cpp \
src/cpp/TodoChecker.cpp \
src/cpp/McCabeChecker.cpp \
src/cpp/McCabeVisitor.cpp \
src/cpp/DecimalChecker.cpp \
src/cpp/LineLengthChecker.cpp \
src/cpp/LinesCountChecker.cpp \
src/cpp/IllegalCallsChecker.cpp \
src/cpp/NaNChecker.cpp \
src/cpp/EqEqChecker.cpp \
src/cpp/UselessOpChecker.cpp \
src/cpp/UnreachableCodeChecker.cpp \
src/cpp/DeprecatedChecker.cpp \
src/cpp/SelectChecker.cpp \
src/cpp/ImplicitListChecker.cpp \
src/cpp/StructChecker.cpp \
src/cpp/LoadSaveChecker.cpp \
src/cpp/OldNotChecker.cpp \
src/cpp/SpacesAroundOpChecker.cpp \
src/cpp/BreaksInLoopChecker.cpp \
src/cpp/NestedBlocksChecker.cpp \
src/cpp/BracketedExpChecker.cpp \
src/cpp/NotNotChecker.cpp \
src/cpp/SpacesInArgsChecker.cpp \
src/cpp/GlobalKeywordChecker.cpp \
src/cpp/StatInCondChecker.cpp \
src/cpp/CommentRatioChecker.cpp \
src/cpp/FunctionArgsOrderChecker.cpp \
src/cpp/FunctionTestReturnChecker.cpp \
src/cpp/ReturnsCountChecker.cpp \
src/cpp/XMLtools.cpp \
src/cpp/SLintXmlResult.cpp \
src/cpp/AnalysisConfiguration.cpp \
src/cpp/AnalysisConfigurationType.cpp \
src/cpp/AnalysisRuleParameterType.cpp \
src/cpp/AnalysisRuleType.cpp \
src/cpp/ExcludedProjectFileType.cpp \
src/cpp/RuleLinkType.cpp \
src/cpp/Standard.cpp \
src/cpp/StandardRuleParameterType.cpp \
src/cpp/StandardRuleParameterValueType.cpp \
src/cpp/StandardRuleType.cpp \
src/cpp/StandardType.cpp \
src/cpp/ToolConfiguration.cpp \
src/cpp/ToolConfigurationType.cpp \
src/cpp/CNESException.cpp \
src/cpp/CNESConfig.cpp \
src/cpp/CNESResult.cpp

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_slint.cpp

libscislint_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/threads/includes/ \
    $(XML_FLAGS) \
    $(SLINT_FLAGS) \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscislint.la
noinst_LTLIBRARIES = libscislint-algo.la

libscislint_algo_la_SOURCES = $(SLINT_CXX_SOURCES)
libscislint_la_SOURCES = $(GATEWAY_CXX_SOURCES)
libscislint_algo_la_CPPFLAGS = $(libscislint_la_CPPFLAGS)

libscislint_la_LDFLAGS = $(AM_LDFLAGS)
libscislint_la_LIBADD = libscislint-algo.la $(XML_LIBS)

# For the code check (splint)
INCLUDE_FLAGS = $(libscislint_la_CPPFLAGS)

#### slint : include files ####
libscislint_la_includedir=$(pkgincludedir)

#### slint : gateway declaration ####
libscislint_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscislint_la_sci_gateway_DATA = sci_gateway/slint_gateway.xml
