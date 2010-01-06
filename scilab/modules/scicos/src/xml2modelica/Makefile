#
#  XML to Modelica
#
#  Copyright (C) 2005 - 2007 Imagine S.A.
#  For more information or commercial use please contact us at www.amesim.com
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#

OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4

OCAMLFLAGS=
OCAMLOPTFLAGS=
LFLAGS=
LIBS=
MLS=xMLTree.ml linenum.ml stringParser.ml stringLexer.ml xMLParser.ml xMLLexer.ml\
	modelicaCodeGenerator.ml xML2Modelica.ml
MLIS=
PLATFORM=$(shell uname -s)
EXE=../../XML2Modelica

.SUFFIXES: .ml .mli .cmo .cmi .cmx .mlp .mpi


.mlp.cmo:
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $(<) > $(@:.cmo=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -impl $(@:.cmo=.tmp)
	rm -f $(@:.cmo=.tmp)

.mpi.cmi:
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -intf $(<) > $(@:.cmi=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -intf $(@:.cmi=.tmp)
	rm -f $(@:.cmi=.tmp)

.mlp.cmx:
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $(<) > $(@:.cmx=.tmp)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c -impl $(@:.cmx=.tmp)
	rm -f $(@:.cmx=.tmp)

.ml.cmo:
	$(OCAMLC) $(OCAMLFLAGS) -c $(<)

.mli.cmi:
	$(OCAMLC) $(OCAMLFLAGS) -c $(<)

.ml.cmx:
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $(<)

%.ml %.mli: %.mly
	ocamlyacc $(<)
	rm -f $(<:.mly=.mli)

%.ml: %.mll
	ocamllex $(<)


all : bytecode

opt: $(MLIS:.mli=.cmi) $(MLS:.ml=.cmx)
	ocamlopt $(OCAMLOPTFLAGS) -o $(EXE) $(LFLAGS) $(LIBS:.cma=.cmxa) $(MLS:.ml=.cmx)

bytecode: $(MLIS:.mli=.cmi) $(MLS:.ml=.cmo)
	ocamlc $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(MLS:.ml=.cmo)

debug: OCAMLFLAGS+=-g
debug: bytecode

prof: OCAMLOPTFLAGS+=-p
prof: opt

depend: $(MLS) $(MLIS)
	touch .depend
	ocamldep $(MLS) $(MLIS) > .depend

-include .depend

clean:
	rm -f *.cmxa
	rm -f *.cm[aiox]
	rm -f *.tmp
	rm -f *.obj
	rm -f *.o
	rm -f stringParser.mli
	rm -f stringLexer.mli
	rm -f xMLParser.mli
	rm -f xMLLexer.mli
	rm -f $(EXE)
	rm -rf doc
