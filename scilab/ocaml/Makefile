OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4
RM=rm


OCAMLFLAGS=
OCAMLOPTFLAGS=
LFLAGS=
LIBS=nums.cma
MLS=parseTree.ml parser.ml lexer.ml\
	precompilation.ml compilation.ml instantiation.ml\
	graphNodeSet.ml symbolicExpression.ml\
	squareSparseMatrix.ml bipartiteGraph.ml hungarianMethod.ml\
	causalityGraph.ml\
	optimization.ml scicosCodeGeneration.ml optimizingCompiler.ml
MLIS=
DOCS=parseTree.mli\
	precompilation.mli compilation.mli instantiation.mli\
	graphNodeSet.mli symbolicExpression.mli\
	squareSparseMatrix.mli bipartiteGraph.mli hungarianMethod.mli\
	causalityGraph.mli\
	optimization.mli scicosCodeGeneration.mli
PLATFORM=$(shell uname -s)
EXE=modelicac


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



all : 
	make depend
	make opt


opt:  $(MLIS:.mli=.cmi) $(MLS:.ml=.cmx)
	@$(OCAMLOPT) -o $(EXE) $(LFLAGS) $(LIBS:.cma=.cmxa) $(MLS:.ml=.cmx)
	@cp -p $(EXE) ../bin/


bytecode: depend $(MLIS:.mli=.cmi) $(MLS:.ml=.cmo)
	@$(OCAMLC) $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(MLS:.ml=.cmo)


debug: OCAMLFLAGS+=-g
debug: bytecode


prof: OCAMLOPTFLAGS+=-p
prof: opt


doc:    $(DOCS)
	@mkdir -p doc
	@$(OCAMLC) -c $(DOCS)
	@ocamldoc -html -d doc $(DOCS)


archive: $(DOCS) $(DOCS:.mli=.ml) optimizingCompiler.ml lexer.mll parser.mly\
README.txt Makefile
	@mkdir -p sources
	@for f in $(DOCS) $(DOCS:.mli=.ml) optimizingCompiler.ml lexer.mll\
	@parser.mly README.txt Makefile;\
	@do\
		cp $$f sources/$$f;\
	done
	@tar cvf - sources | gzip > archive.tar.gz


depend: $(MLS) $(MLIS)
	@touch .depend
	@$(OCAMLDEP) $(MLS) $(MLIS) > .depend


-include .depend


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
	@$(RM) -f .depend


distclean : clean
	@$(RM) -rf doc
super_distclean : distclean
