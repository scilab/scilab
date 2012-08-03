#  Scicos
#
#  Copyright (C) DIGITEO - 2010 - Allan CORNET
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# See the file ./license.txt

!IF "$(OCAMLLIB)" == ""
OCAMLPATH=C:\Program Files\Objective Caml
!ELSE
OCAMLPATH=$(OCAMLLIB)\..
!ENDIF

OCAMLPATHBIN=$(OCAMLPATH)\bin
OCAMLPATHLIB=$(OCAMLPATH)\lib
OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4
OCAMLYACC=ocamlyacc
OCAMLLEX=ocamllex
RM=del
EXEC=modelicat.exe
INCLUDEPATHS=-I ./compilation -I ./exceptionHandling -I ./parsing -I ./instantiation -I ./translation

all::
	$(OCAMLYACC) parsing/parser.mly
	$(RM) parsing\parser.mli
	$(OCAMLLEX) parsing/lexer.mll
	$(OCAMLC) $(INCLUDEPATHS) -c parsing/syntax.ml parsing/parser.ml
	$(OCAMLC) $(INCLUDEPATHS) -c parsing/parser.ml parsing/lexer.ml
	$(OCAMLC) $(INCLUDEPATHS) -c parsing/lexer.ml parsing/linenum.ml
	$(OCAMLC) $(INCLUDEPATHS) -c parsing/linenum.ml compilation/types.ml
	$(OCAMLC) $(INCLUDEPATHS) -c compilation/types.ml compilation/nameResolve.ml
	$(OCAMLC) $(INCLUDEPATHS) -c compilation/nameResolve.ml instantiation/instantiation.ml
	$(OCAMLC) $(INCLUDEPATHS) -c instantiation/instantiation.ml exceptionHandling/msgDico.ml
	$(OCAMLC) $(INCLUDEPATHS) -c exceptionHandling/msgDico.ml exceptionHandling/errorDico.ml
	$(OCAMLC) $(INCLUDEPATHS) -c exceptionHandling/errorDico.ml exceptionHandling/exceptHandler.ml
	$(OCAMLC) $(INCLUDEPATHS) -c exceptionHandling/exceptHandler.ml translation/libraryManager.ml
	$(OCAMLC) $(INCLUDEPATHS) -c translation/libraryManager.ml translation/codeGeneration.ml
	$(OCAMLC) $(INCLUDEPATHS) -c translation/codeGeneration.ml 
	$(OCAMLC) $(INCLUDEPATHS) -c translation/versiondate.ml translation/translator.ml
	$(OCAMLC) $(INCLUDEPATHS) -c translation/translator.ml parsing/syntax.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c parsing/syntax.ml parsing/parser.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c parsing/parser.ml parsing/lexer.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c parsing/lexer.ml parsing/linenum.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c parsing/linenum.ml compilation/types.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c compilation/types.ml compilation/nameResolve.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c compilation/nameResolve.ml instantiation/instantiation.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c instantiation/instantiation.ml exceptionHandling/msgDico.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c exceptionHandling/msgDico.ml exceptionHandling/errorDico.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c exceptionHandling/errorDico.ml exceptionHandling/exceptHandler.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c exceptionHandling/exceptHandler.ml translation/libraryManager.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c translation/libraryManager.ml translation/codeGeneration.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c translation/codeGeneration.ml 
	$(OCAMLOPT) $(INCLUDEPATHS) -c translation/versiondate.ml translation/translator.ml
	$(OCAMLOPT) $(INCLUDEPATHS) -c translation/translator.ml
	$(OCAMLOPT) -o $(EXEC) $(INCLUDEPATHS) nums.cmxa ./parsing/syntax.cmx ./parsing/parser.cmx \
	    ./parsing/lexer.cmx ./parsing/linenum.cmx ./compilation/types.cmx ./compilation/nameResolve.cmx \
	    ./instantiation/instantiation.cmx ./exceptionHandling/msgDico.cmx ./exceptionHandling/errorDico.cmx \
	    ./exceptionHandling/exceptHandler.cmx ./translation/libraryManager.cmx ./translation/codeGeneration.cmx \
	    ./translation/versiondate.cmx ./translation/translator.cmx 
	@copy $(EXEC) ..\..\..\..\bin\$(EXEC)
	$(RM) parsing\parser.ml
	$(RM) parsing\lexer.ml
clean::
	@-del /s *.cmi
	@-del /s *.cmo
	@-del /s *.cmx
	@-del /s *.cma
	@-del /s *.obj
	@-del $(EXEC)
	
distclean::
	@-del /s *.cmi
	@-del /s *.cmo
	@-del /s *.cmx
	@-del /s *.cma
	@-del /s *.obj
	@-del $(EXEC)
	@-del ..\..\..\..\bin\$(EXEC)