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
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

all::
	$(OCAMLC) -g  -c syntax.ml
	$(OCAMLC) -g  -c linenum.ml
	$(OCAMLYACC) parser.mly
	$(RM) parser.mli
	$(OCAMLC) -g -c parser.ml
	$(OCAMLC) -g -c lexer.ml
	$(OCAMLC) -g -a -o parsing.cma syntax.cmo linenum.cmo parser.cmo lexer.cmo
	
clean::
	@-$(RM) *.cmo
	@-$(RM) *.cma
	@-$(RM) *.cmi
	
distclean::
	@-$(RM) *.cmo
	@-$(RM) *.cma
	@-$(RM) *.cmi
	
