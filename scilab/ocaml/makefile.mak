OCAMLPATH=C:\Program Files\Objective Caml
OCAMLPATHBIN=$(OCAMLPATH)\bin
OCAMLPATHLIB=$(OCAMLPATH)\lib
OCAMLC=ocamlc
OCAMLOPT=ocamlopt
OCAMLDEP=ocamldep
CAMLP4=camlp4
OCAMLYACC=ocamlyacc
OCAMLLEX=ocamllex
RM=del
EXEC=modelicac.exe

MLS=parseTree.ml parser.ml lexer.ml\
	precompilation.ml compilation.ml instantiation.ml\
	graphNodeSet.ml symbolicExpression.ml\
	squareSparseMatrix.ml bipartiteGraph.ml hungarianMethod.ml\
	causalityGraph.ml\
	optimization.ml optimizingCompiler.ml\
	scicosCodeGeneration.ml scicosOptimizingCompiler.ml
    
CMACMO=nums.cma parseTree.cmo parser.cmo \
       lexer.cmo precompilation.cmo compilation.cmo \
       instantiation.cmo graphNodeSet.cmo symbolicExpression.cmo \
       squareSparseMatrix.cmo bipartiteGraph.cmo hungarianMethod.cmo \
       causalityGraph.cmo optimization.cmo scicosCodeGeneration.cmo \
       optimizingCompiler.cmo

all:: step1 step2 step3 step4


step1: 
	@"$(OCAMLPATHBIN)\$(OCAMLYACC)" parser.mly
	@$(RM) parser.mli
	@"$(OCAMLPATHBIN)\$(OCAMLLEX)" lexer.mll

step2:
	@"$(OCAMLPATHBIN)\$(OCAMLDEP)" $(MLS)

step3: 
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c parseTree.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c parseTree.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c parser.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c lexer.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c precompilation.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c precompilation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c compilation.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c compilation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c instantiation.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c instantiation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c graphNodeSet.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c graphNodeSet.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c symbolicExpression.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c symbolicExpression.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c squareSparseMatrix.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c squareSparseMatrix.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c bipartiteGraph.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c bipartiteGraph.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c hungarianMethod.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c hungarianMethod.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c causalityGraph.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c causalityGraph.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimization.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimization.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimizingCompiler.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimizingCompiler.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosCodeGeneration.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosCodeGeneration.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosOptimizingCompiler.ml
	
step4:
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -o $(EXEC) $(CMACMO) scicosOptimizingCompiler.ml
	@copy  $(EXEC) ..\bin\$(EXEC)
	
clean::
	-$(RM)  *.cmi
	-$(RM)  *.cmo
	-$(RM)  *.cmx
	-$(RM)  *.obj
	-$(RM)  parser.ml
	-$(RM)  lexer.ml
	-$(RM)  *.exe
	-$(RM)  ..\bin\$(EXEC)
	
distclean::
  -$(RM)  *.cmi
	-$(RM)  *.cmo
	-$(RM)  *.cmx
	-$(RM)  *.obj
	-$(RM)  parser.ml
	-$(RM)  lexer.ml
	-$(RM)  *.exe