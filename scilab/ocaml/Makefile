OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4

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


all : opt doc

opt: bytecode $(MLIS:.mli=.cmi) $(MLS:.ml=.cmx)
	ocamlopt -o $(EXE) $(LFLAGS) $(LIBS:.cma=.cmxa) $(MLS:.ml=.cmx)

bytecode: depend $(MLIS:.mli=.cmi) $(MLS:.ml=.cmo)
	ocamlc $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(MLS:.ml=.cmo)

debug: OCAMLFLAGS+=-g
debug: bytecode

prof: OCAMLOPTFLAGS+=-p
prof: opt

doc:	$(DOCS)
	mkdir -p doc
	ocamlc -c $(DOCS)
	ocamldoc -html -d doc $(DOCS)

archive: $(DOCS) $(DOCS:.mli=.ml) optimizingCompiler.ml lexer.mll parser.mly\
README.txt Makefile
	mkdir -p sources
	for f in $(DOCS) $(DOCS:.mli=.ml) optimizingCompiler.ml lexer.mll\
	parser.mly README.txt Makefile;\
	do\
		cp $$f sources/$$f;\
	done
	tar cvf - sources | gzip > archive.tar.gz

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
	rm -f parser.mli
	rm -f lexer.mli
	rm -f parser.ml
	rm -f lexer.ml
	rm -f $(EXE)

distclean : clean
	rm -rf doc
