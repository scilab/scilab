function [tree]=sci_isspace(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isspace()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_isspace()
// V.C.

A = getrhs(tree)

if A.vtype==String then
  A = Funcall("asciimat",1,Rhs(A),list(Variable("",A.infer)))
  tree = Operation("==",list(A,Cste(32)),tree.lhs)
  tree.out(1).dims=A.dims
  tree.out(1).type=Type(Boolean,Real)
elseif or(A.vtype==[Double,Boolean]) then
  tree.name="zeros"
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Boolean,Real)
else
  scitree=tree
  scitree.name="mtlb_isspace"
  repl1=tree
  repl1.rhs(1) = Funcall("asciimat",1,Rhs(repl1.rhs(1)),list())
  repl1 = Operation("==",list(repl1.rhs(1),Cste(32)),tree.lhs)
  repl2=tree
  repl2.name="zeros"
  
  repl_poss(scitree,..
      repl1,A,"is a character string matrix",..
      repl2,A,"is not a character string matrix")

  tree=scitree
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Boolean,Real)
end

endfunction
