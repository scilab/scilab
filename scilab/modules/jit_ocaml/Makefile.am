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
modulename=jit_ocaml


#### jit_ocaml : Conf files ####
libscijit_ocaml_la_rootdir = $(mydatadir)
libscijit_ocaml_la_root_DATA =  license.txt


#### jit_ocaml : init scripts ####
libscijit_ocaml_la_etcdir = $(mydatadir)/etc
libscijit_ocaml_la_etc_DATA = etc/jit_ocaml.quit \
			  etc/jit_ocaml.start

include $(top_srcdir)/Makefile.incl.am


JIT_OCAML_CPP_SOURCES = \
			src/cpp/scicaml_ast2string.cpp \
			src/cpp/scicaml_string2ast.cpp

#JIT_OCAML_C_SOURCES = \
#			src/c/jit_callback.c

libscijit_ocaml_la_CFLAGS = -I$(srcdir)/includes

libscijit_ocaml_la_CPPFLAGS = \
	-I$(srcdir)/includes \
        -I$(top_srcdir)/libs/MALLOC/includes \
	-I$(top_srcdir)/modules/elementary_functions/includes \
	-I$(top_srcdir)/modules/localization/includes \
	-I$(top_srcdir)/modules/operations/includes \
	-I$(top_srcdir)/modules/symbol/includes \
	-I$(top_srcdir)/modules/ast/includes \
	-I$(top_srcdir)/modules/types/includes \
	-I$(top_srcdir)/modules/parse/includes \
	-I$(top_srcdir)/modules/system_env/includes \
	-I$(top_srcdir)/modules/functions_manager/includes \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/threads/includes \
        -I$(top_srcdir)/modules/api_scilab/includes \
        -I$(top_srcdir)/modules/console/includes \
        -I$(top_srcdir)/modules/output_stream/includes \
        -I$(top_srcdir)/libs/dynamiclibrary/includes \
	-I src/cpp \
	$(EIGEN_CPPFLAGS) \
	$(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscijit_ocaml.la
#BUILT_SOURCES = libscijit_ocaml-runtime.a
#
libscijit_ocaml_la_LIBADD= libscijit_ocaml-runtime.o
libscijit_ocaml_la_SOURCES = $(JIT_OCAML_CPP_SOURCES)


OCAMLOPT=ocamlopt
OCAMLYACC=ocamlyacc
OCAMLLEX=ocamllex
OCAMLC_WHERE := `$(OCAMLOPT) -where`


jit_ocaml.o: src/c/jit_ocaml.c
	ocamlopt -c -I includes -fPIC src/c/jit_ocaml.c

OCAML_AST_MLS = \
	src/ocaml/scilabAst.ml \
	src/ocaml/scilabTypes.ml \
	src/ocaml/scilabAstPrinter.ml
OCAML_AST_MLIS = \
	src/ocaml/scilabAstPrinter.mli

OCAML_SCILAB_MLS = \
	src/ocaml/scilabString2Ast.ml \
	src/ocaml/scilabAst2String.ml \
	src/ocaml/scilabCallbacks.ml
OCAML_SCILAB_MLIS = \
	src/ocaml/scilabString2Ast.mli \
	src/ocaml/scilabAst2String.mli \
	src/ocaml/scilabCallbacks.mli


OCAML_MLS = \
	$(OCAML_AST_MLS) \
	$(OCAML_SCILAB_MLS)

OCAML_MLIS = \
	$(OCAML_AST_MLIS) \
	$(OCAML_SCILAB_MLIS)

OCAML_PARSER_MLS = \
	src/ocaml/scilabAst.ml \
	src/ocaml/scilabAstPrinter.ml \
	src/yacc/scilabParser.ml \
	src/lex/scilabLexer.ml \
	src/ocaml/scilabParserMain.ml

OCAML_PARSER_CMXS = $(OCAML_PARSER_MLS:.ml=.cmx)

OCAML_CMIS = $(OCAML_MLS:.ml=.cmi) $(OCAML_MLIS:.mli=.cmi)
OCAML_CMXS = $(OCAML_MLS:.ml=.cmx)
OCAML_OBJS = $(OCAML_MLS:.ml=.o)

OCAML_INCL= -I src/ocaml -I src/yacc -I src/lex
OCAML_PARSER_INCL = -I src/yacc
OPTFLAGS = -c -fPIC $(OCAML_INCL)


CLEANFILES= \
	$(OCAML_CMIS)  \
	$(OCAML_CMXS)  \
	$(OCAML_OBJS)  \
	jit_ocaml.o \
	libasmrun.a \
	ocaml_parser \
	src/lex/*.cm* \
	src/lex/*.o \
	src/yacc/*.cm* \
	src/yacc/*.o \
	src/lex/scilabLexer.ml \
	src/yacc/scilabParser.ml \
	src/yacc/scilabParser.mli \
	scilab_parser \
	macros/lib


scilab_parser : $(OCAML_PARSER_CMXS)
	$(OCAMLOPT) -o scilab_parser $(OCAML_PARSER_CMXS)


libscijit_ocaml-runtime.o: $(OCAML_CMXS) jit_ocaml.o libasmrun.a
	$(OCAMLOPT) -o libscijit_ocaml-runtime.o -output-obj $(OCAML_CMXS)

libasmrun.a:
	cp $(OCAMLC_WHERE)/libasmrun.a .


depend_ocaml:
	$(OCAMLDEP) -native $(OCAML_INCL) $(OCAML_MLS) $(OCAML_MLIS) $(OCAML_PARSER_MLS) > .depend_ocaml
include .depend_ocaml

########## COMMON
SUFFIXES += .ml .mli .mll .mly .cmi .cmx

.ml.cmx:
	$(OCAMLOPT) $(OPTFLAGS) -c $<

.mll.cmx:
	$(OCAMLLEX) $<
	$(OCAMLOPT) $(OPTFLAGS) -c $*.ml

.mli.cmi:
	$(OCAMLOPT) $(OPTFLAGS) -c $<

.mll.ml:
	$(OCAMLLEX) $<

.mly.ml:
	$(OCAMLYACC) -v $<

.mly.cmx:
	$(OCAMLYACC) $<
	$(OCAMLOPT) $(OPTFLAGS) -c $*.mli
	$(OCAMLOPT) $(OPTFLAGS) -c $*.ml

.mly.cmi:
	$(OCAMLYACC) -v $<
	$(OCAMLOPT) $(OPTFLAGS) -c $*.mli

