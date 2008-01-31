#  Scicos
#
#  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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

OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4
RM=rm


OCAMLFLAGS=
OCAMLOPTFLAGS=
LFLAGS=
LIBS=nums.cma
MLS=parseTree.ml linenum.ml parser.ml lexer.ml\
	precompilation.ml compilation.ml instantiation.ml\
	graphNodeSet.ml symbolicExpression.ml\
	squareSparseMatrix.ml bipartiteGraph.ml hungarianMethod.ml\
	causalityGraph.ml\
	optimization.ml  xMLCodeGeneration.ml optimizingCompiler.ml\
	scicosCodeGeneration.ml scicosOptimizingCompiler.ml
MLIS=
DOCS=parseTree.mli\
	precompilation.mli compilation.mli instantiation.mli\
	graphNodeSet.mli symbolicExpression.mli \
	squareSparseMatrix.mli bipartiteGraph.mli hungarianMethod.mli \
	causalityGraph.mli \
	optimization.mli \
	xMLCodeGeneration.mli \
	optimizingCompiler.mli \
	scicosCodeGeneration.mli 
PLATFORM=$(shell uname -s)

EXE=../bin/modelicac
#TRANSLATOR=../../bin/translator


.SUFFIXES: .ml .mli .cmo .cmi .cmx .mlp .mpi



.mlp.cmo:
	@$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $(<) > $(@:.cmo=.tmp)
	@$(OCAMLC) $(OCAMLFLAGS) -c -impl $(@:.cmo=.tmp)
	@$(RM) -f $(@:.cmo=.tmp)


.mpi.cmi:
	@$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -intf $(<) > $(@:.cmi=.tmp)
	@$(OCAMLC) $(OCAMLFLAGS) -c -intf $(@:.cmi=.tmp)
	@$(RM) -f $(@:.cmi=.tmp)


.mlp.cmx:
	@$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $(<) > $(@:.cmx=.tmp)
	@$(OCAMLOPT) $(OCAMLOPTFLAGS) -c -impl $(@:.cmx=.tmp)
	@$(RM) -f $(@:.cmx=.tmp)


.ml.cmo:
	@$(OCAMLC) $(OCAMLFLAGS) -c $(<)


.mli.cmi:
	@$(OCAMLC) $(OCAMLFLAGS) -c $(<)


.ml.cmx:
	@$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $(<)


%.ml %.mli: %.mly
	@ocamlyacc $(<)
	@$(RM) -f $(<:.mly=.mli)


%.ml: %.mll
	@ocamllex $(<)


all : depend $(EXE)

$(EXE): $(MLIS:.mli=.cmi) $(MLS:.ml=.cmx)
	@$(OCAMLOPT) -o $(EXE) $(LFLAGS) $(LIBS:.cma=.cmxa) $(MLS:.ml=.cmx)

bytecode: depend $(MLIS:.mli=.cmi) $(MLS:.ml=.cmo)
	@$(OCAMLC) $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(MLS:.ml=.cmo)

opt:
	make all

debug:  
	make all OCAMLFLAGS="-g $(OCAMLFLAGS)" 

prof:  
	make all OCAMLFLAGS="-p $(OCAMLFLAGS)" 

doc:    $(DOCS)
	@mkdir -p doc
	@$(OCAMLC) -c $(DOCS)
	@ocamldoc -html -d doc $(DOCS)


archive: $(DOCS) $(DOCS:.mli=.ml) scicosOptimizingCompiler.ml \
linenum.mll lexer.mll parser.mly README.txt Makefile
	@mkdir -p sources
	@for f in $(DOCS) $(DOCS:.mli=.ml) scicosOptimizingCompiler.ml linenum.mll lexer.mll parser.mly README.txt Makefile;\
	@do\
		cp $$f sources/$$f;\
	done
	@tar cvf - sources | gzip > archive.tar.gz

depend : .depend 

.depend: $(MLS) $(MLIS)
	@echo "regenerate .depend"
	@touch .depend
	@$(OCAMLDEP) $(MLS) $(MLIS) > .depend


include .depend

$(TRANSLATOR):
	@cd modelica;$(MAKE) depend;$(MAKE);mv translation/translator $(TRANSLATOR)

clean:
	@$(RM) -f *.cmxa
	@$(RM) -f *.cm[aiox]
	@$(RM) -f *.tmp
	@$(RM) -f *.obj
	@$(RM) -f *.o
	@$(RM) -f parser.mli
	@$(RM) -f lexer.mli
	@$(RM) -f parser.ml
	@$(RM) -f lexer.ml
	@$(RM) -f $(EXE)
#	@cd modelica;$(MAKE) clean; 
#	@$(RM) -f $(TRANSLATOR)

distclean : clean
	@$(RM) -rf doc $(EXE) 

super_distclean : distclean
