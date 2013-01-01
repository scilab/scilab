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


# We generate:
# - libscijit_ocaml.la : a bridge between the C++ AST and a serialized
#      representation
# - libocpsci.la : some C++ code to be linked statically, to manipulate C++
#      objects from OCaml
# - ocamlcode.o : a module containing all the OCaml code, to be linked
#      statically with libocpsci.la

#### Target ######
modulename=jit_ocaml

#### jit_ocaml : Conf files ####
libscijit_ocaml_la_rootdir = $(mydatadir)
libscijit_ocaml_la_root_DATA =  license.txt

OCAMLLIB=$(shell ocamlc -where)


#### jit_ocaml : init scripts ####
libscijit_ocaml_la_etcdir = $(mydatadir)/etc
libscijit_ocaml_la_etc_DATA = etc/jit_ocaml.quit \
			  etc/jit_ocaml.start

include $(top_srcdir)/Makefile.incl.am

JIT_OCAML_CPP_SOURCES = \
			src/cpp/scicaml_ast2string.cpp \
			src/cpp/scicaml_string2ast.cpp

libscijit_ocaml_la_CFLAGS = \
   -I$(srcdir)/includes \
   -I $(OCAMLLIB)

CLASSIC_CPPFLAGS= \
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

libscijit_ocaml_la_CPPFLAGS = $(CLASSIC_CPPFLAGS)

pkglib_LTLIBRARIES = libscijit_ocaml.la
#BUILT_SOURCES = ocamlcode.o
#
libscijit_ocaml_la_LIBADD= ocamlcode.o
libscijit_ocaml_la_SOURCES = $(JIT_OCAML_CPP_SOURCES)

noinst_LTLIBRARIES= libocpsci.la
libocpsci_la_SOURCES= \
    src/c/jit_ocaml.c \
    src/cpp/ocpsci_gw.cpp \
    src/cpp/ocpsci_stubs.cpp

EXTRA_libocpsci_la_DEPENDENCIES=ocamlcode.o

libocpsci_la_CFLAGS = \
   -I$(srcdir)/includes \
   -I $(OCAMLLIB)
libocpsci_la_CPPFLAGS = $(CLASSIC_CPPFLAGS) -I $(OCAMLLIB)

OCAMLOPT=ocamlopt
OCAMLYACC=ocamlyacc
OCAMLLEX=ocamllex

OCAML_AST_MLS = \
	src/ocaml/scilabMisc.ml \
	src/ocaml/scilabValue.ml \
	src/ocaml/scilabContext.ml \
	src/ocaml/scilabAst.ml \
	src/ocaml/scilabTypes.ml \
	src/ocaml/scilabAstPrinter.ml
OCAML_AST_MLIS = \
	src/ocaml/scilabValue.mli \
	src/ocaml/scilabContext.mli \
	src/ocaml/scilabAstPrinter.mli

OCAML_SCILAB_MLS = \
	src/ocaml/scilabString2Ast.ml \
	src/ocaml/scilabAst2String.ml \
	src/ocaml/scilabInternalType.ml \
	src/ocaml/scilabInterp.ml \
	src/ocaml/scilabCallbacks.ml
OCAML_SCILAB_MLIS = \
	src/ocaml/scilabString2Ast.mli \
	src/ocaml/scilabAst2String.mli \
	src/ocaml/scilabInterp.mli \
	src/ocaml/scilabCallbacks.mli

OCAML_MLS = \
	$(OCAML_AST_MLS) \
	$(OCAML_SCILAB_MLS)

OCAML_MLIS = \
	$(OCAML_AST_MLIS) \
	$(OCAML_SCILAB_MLIS)

OCAML_CMIS = $(OCAML_MLS:.ml=.cmi) $(OCAML_MLIS:.mli=.cmi)
OCAML_CMXS = $(OCAML_MLS:.ml=.cmx)
OCAML_OBJS = $(OCAML_MLS:.ml=.o)

OCAML_INCL= -I src/ocaml
OPTFLAGS = -c -fPIC $(OCAML_INCL)


CLEANFILES= \
	$(OCAML_CMIS)  \
	$(OCAML_CMXS)  \
	$(OCAML_OBJS)  \
	jit_ocaml.o \
	libasmrun.a libunix.a

ocamlcode.o: $(OCAML_CMXS)
	$(OCAMLOPT) -o ocamlcode.o -output-obj unix.cmxa $(OCAML_CMXS)

libasmrun.a:
	cp $(OCAMLLIB)/libasmrun.a .
libunix.a:
	cp $(OCAMLLIB)/libunix.a .


depend_ocaml:
	$(OCAMLDEP) -native $(OCAML_INCL) $(OCAML_MLS) $(OCAML_MLIS) > .depend_ocaml
include .depend_ocaml

########## COMMON
SUFFIXES += .ml .mli .mll .mly .cmi .cmx

.ml.cmx:
	$(OCAMLOPT) $(OPTFLAGS) -c $<

.mli.cmi:
	$(OCAMLOPT) $(OPTFLAGS) -c $<

.mll.ml:
	$(OCAMLLEX) $<

.mly.ml:
	$(OCAMLYACC) $<

