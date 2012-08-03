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
EXEC=modelicac.exe
OCAMLLIBS=nums.cmxa
PARSER_SRC=parser.mly
LEXER_SRC=lexer.mll

MLS=parseTree.ml linenum.ml parser.ml lexer.ml\
	precompilation.ml compilation.ml instantiation.ml\
	graphNodeSet.ml symbolicExpression.ml\
	squareSparseMatrix.ml bipartiteGraph.ml hungarianMethod.ml\
	causalityGraph.ml\
	optimization.ml xMLCodeGeneration.ml optimizingCompiler.ml\
	scicosCodeGeneration.ml scicosOptimizingCompiler.ml
    
CMACMO=linenum.cmo nums.cma parseTree.cmo parser.cmo \
       lexer.cmo precompilation.cmo compilation.cmo \
       instantiation.cmo graphNodeSet.cmo symbolicExpression.cmo \
       squareSparseMatrix.cmo bipartiteGraph.cmo hungarianMethod.cmo \
       causalityGraph.cmo optimization.cmo scicosCodeGeneration.cmo \
       xMLCodeGeneration.cmo optimizingCompiler.cmo

CMX=parseTree.cmx linenum.cmx parser.cmx lexer.cmx \
	precompilation.cmx compilation.cmx instantiation.cmx \
	graphNodeSet.cmx symbolicExpression.cmx squareSparseMatrix.cmx \
	bipartiteGraph.cmx hungarianMethod.cmx causalityGraph.cmx \
	optimization.cmx xMLCodeGeneration.cmx optimizingCompiler.cmx \
	scicosCodeGeneration.cmx scicosOptimizingCompiler.cmx

all:: step1 step2 step3 step4 step5 step6


step1: 
	@"$(OCAMLPATHBIN)\$(OCAMLLEX)" linenum.mll
	@"$(OCAMLPATHBIN)\$(OCAMLYACC)" $(PARSER_SRC)
	@$(RM) parser.mli
	@"$(OCAMLPATHBIN)\$(OCAMLLEX)" $(LEXER_SRC)
	
	
step2:
	@"$(OCAMLPATHBIN)\$(OCAMLDEP)" $(MLS)
	
step3: 
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c linenum.ml
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
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c xMLCodeGeneration.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c xMLCodeGeneration.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimizingCompiler.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c optimizingCompiler.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosCodeGeneration.mli
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosCodeGeneration.ml
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -c scicosOptimizingCompiler.ml
	
	
step4:
	@"$(OCAMLPATHBIN)\$(OCAMLC)" -o $(EXEC) $(CMACMO) scicosOptimizingCompiler.ml
	
	
step5:
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c linenum.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c parseTree.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c parser.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c lexer.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c precompilation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c compilation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c instantiation.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c graphNodeSet.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c symbolicExpression.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c squareSparseMatrix.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c bipartiteGraph.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c hungarianMethod.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c causalityGraph.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c optimization.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c xMLCodeGeneration.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c scicosCodeGeneration.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c optimizingCompiler.ml
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -c scicosOptimizingCompiler.ml
	
	
step6:
	@"$(OCAMLPATHBIN)\$(OCAMLOPT)" -o $(EXEC) $(OCAMLLIBS) $(CMX)
	@copy  $(EXEC) ..\..\..\..\bin\$(EXEC)
	
clean::
	@-$(RM)  *.cmi
	@-$(RM)  *.cmo
	@-$(RM)  *.cmx
	@-$(RM)  *.obj
	@-$(RM)  parser.ml
	@-$(RM)  lexer.ml
	@-$(RM)  linenum.ml
	@-$(RM)  *.exe
	@-$(RM)  ..\..\..\..\bin\$(EXEC)
	
	
distclean::
	@-$(RM)  *.cmi
	@-$(RM)  *.cmo
	@-$(RM)  *.cmx
	@-$(RM)  *.obj
	@-$(RM)  parser.ml
	@-$(RM)  lexer.ml
	@-$(RM)  linenum.ml
	@-$(RM)  *.exe
	
