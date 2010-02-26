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
EXEC=modelicat.exe

all::
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -c ..\compilation\types.ml
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -c codeGeneration.ml
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -c ..\parsing\lexer.ml
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -c libraryManager.ml
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -c versiondate.ml
	$(OCAMLC) -g -I ..\parsing -I ..\compilation -I ..\instantiation -I ..\exceptionHandling -o $(EXEC)  ..\parsing\parsing.cma ../compilation/compilation.cma ../instantiation/instantiation.cma ../exceptionHandling/exceptionHandling.cma unix.cma codeGeneration.cmo libraryManager.cmo versiondate.cmo translator.ml
	@copy  $(EXEC) ..\..\..\..\..\bin\$(EXEC)

clean::
	@-$(RM) *.cmo
	@-$(RM) *.cma
	@-$(RM) *.cmi
	@-$(RM) $(EXEC)
	@-$(RM) ..\..\..\..\bin\$(EXEC)
	
distclean::
	@-$(RM) *.cmo
	@-$(RM) *.cma
	@-$(RM) *.cmi

