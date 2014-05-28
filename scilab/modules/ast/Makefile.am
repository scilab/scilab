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

pkglib_LTLIBRARIES 	= libsciast.la

libsciast_la_SOURCES = \
	src/cpp/runvisitor.cpp \
	src/cpp/visitor_common.cpp \
	src/cpp/printvisitor.cpp \
	src/cpp/debugvisitor.cpp \
	src/cpp/mutevisitor.cpp \
	src/cpp/shortcutvisitor.cpp \
	src/cpp/scilabexception.cpp \
	src/cpp/macrovarvisitor.cpp \
	src/cpp/expHistory.cpp

libsciast_la_CPPFLAGS = \
	-Iincludes \
	-Iincludes/exps \
	-Isrc/cpp \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/symbol/includes \
	-I$(top_srcdir)/modules/types/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/console/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes \
	-I$(top_srcdir)/modules/operations/includes \
	-I$(top_srcdir)/modules/output_stream/includes \
	-I$(top_srcdir)/modules/system_env/includes \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/threads/includes \
	-I$(top_srcdir)/modules/localization/includes \
    $(EIGEN_CPPFLAGS) \
	$(AM_CPPFLAGS)


libsciast_la_includedir=$(pkgincludedir)
libsciast_la_include_HEADERS =


#### Target ######
modulename=ast

####io : init scripts ####
libsciast_la_etcdir = $(mydatadir)/etc
libsciast_la_etc_DATA = etc/ast.quit etc/ast.start


#### ast : include files ####
libsciast_scilab_la_includedir=$(pkgincludedir)
libsciast_scilab_la_include_HEADERS = \
includes/exps/alldec.hxx \
includes/exps/allexp.hxx \
includes/exps/all.hxx \
includes/exps/allvar.hxx \
includes/exps/anydecs.hxx \
includes/exps/arraylistexp.hxx \
includes/exps/arraylistvar.hxx \
includes/exps/assignexp.hxx \
includes/exps/assignlistexp.hxx \
includes/exps/ast.hxx \
includes/exps/boolexp.hxx \
includes/exps/breakexp.hxx \
includes/exps/callexp.hxx \
includes/exps/caseexp.hxx \
includes/exps/cellcallexp.hxx \
includes/exps/cellexp.hxx \
includes/exps/colonvar.hxx \
includes/exps/commentexp.hxx \
includes/exps/constexp.hxx \
includes/exps/continueexp.hxx \
includes/exps/controlexp.hxx \
includes/exps/dec.hxx \
includes/exps/decls.hxx \
includes/exps/decs.hxx \
includes/exps/dollarvar.hxx \
includes/exps/doubleexp.hxx \
includes/exps/exp.hxx \
includes/exps/fieldexp.hxx \
includes/exps/floatexp.hxx \
includes/exps/forexp.hxx \
includes/exps/functiondec.hxx \
includes/exps/ifexp.hxx \
includes/exps/intexp.hxx \
includes/exps/listexp.hxx \
includes/exps/location.hxx \
includes/exps/logicalopexp.hxx \
includes/exps/mathexp.hxx \
includes/exps/matrixexp.hxx \
includes/exps/matrixlineexp.hxx \
includes/exps/nilexp.hxx \
includes/exps/notexp.hxx \
includes/exps/opexp.hxx \
includes/exps/returnexp.hxx \
includes/exps/selectexp.hxx \
includes/exps/seqexp.hxx \
includes/exps/simplevar.hxx \
includes/exps/stringexp.hxx \
includes/exps/token.hxx \
includes/exps/transposeexp.hxx \
includes/exps/trycatchexp.hxx \
includes/exps/vardec.hxx \
includes/exps/var.hxx \
includes/exps/whileexp.hxx \
includes/debugvisitor.hxx \
includes/deserializervisitor.hxx \
includes/dummyvisitor.hxx \
includes/dynlib_ast_tools.hxx \
includes/execvisitor.hxx \
includes/expHistory.hxx \
includes/macrovarvisitor.hxx \
includes/mutevisitor.hxx \
includes/printvisitor.hxx \
includes/runvisitor.hxx \
includes/scilabexception.hxx \
includes/serializervisitor.hxx \
includes/shortcutvisitor.hxx \
includes/stepvisitor.hxx \
includes/timedvisitor.hxx \
includes/visitor.hxx \
includes/visitor_common.hxx

include $(top_srcdir)/Makefile.incl.am
