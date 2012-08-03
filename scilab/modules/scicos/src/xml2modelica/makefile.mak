#  Scicos
#
#  Copyright (C) INRIA - scilab 
#  Copyright (C) DIGITEO - 2009 - Allan CORNET
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
EXEC=xml2modelica.exe

all::
	$(OCAMLOPT) -c xMLTree.ml
	$(OCAMLOPT) -c linenum.ml
	$(OCAMLOPT) -c stringParser.ml
	$(OCAMLOPT) -c stringLexer.ml
	$(OCAMLOPT) -c xMLParser.ml
	$(OCAMLOPT) -c xMLLexer.ml
	$(OCAMLOPT) -c modelicaCodeGenerator.ml
	$(OCAMLOPT) -c xML2Modelica.ml
	$(OCAMLOPT) -o $(EXEC) xMLTree.cmx linenum.cmx stringParser.cmx stringLexer.cmx xMLParser.cmx xMLLexer.cmx modelicaCodeGenerator.cmx xML2Modelica.cmx
	@copy  $(EXEC) ..\..\..\..\bin\$(EXEC)
	
clean::
	@-$(RM)  *.cmi
	@-$(RM)  *.cmo
	@-$(RM)  *.exe
	@-$(RM)  ..\..\..\..\bin\$(EXEC)
	
distclean::
	@-$(RM)  *.cmi
	@-$(RM)  *.cmo
	@-$(RM)  *.exe

