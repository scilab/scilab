function [tree]=sci_lower(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab lower()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_lower()
// V.C.

opt=part(tree.name,1)

A = getrhs(tree)
if A.vtype==String then
  tree.name="convstr"
  tree.rhs=Rhs(A,opt)
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
elseif A.vtype==Unknown then
  scitree=tree
  scitree.name="mtlb_lower"
  repl1=tree
  repl1.name="convstr"
  repl1.rhs=Rhs(A,opt)
  repl_poss(scitree,..
      repl1,A,"is a character string matrix",..
      A,A,"is not a character string matrix")
  tree=scitree
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
else
  if typeof(A)=="funcall" then
    A.lhs=tree.lhs
  elseif typeof(A)=="operation" then
    A.out=tree.lhs
  end
  tree=A
end
endfunction
