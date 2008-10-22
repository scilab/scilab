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
  tree.name="mtlb_isspace"
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Boolean,Real)
end

endfunction
