function [tree]=sci_rcond(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab rcond()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_rcond()
// V.C.

A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs(A)

// If A can be an empty matrix...
if ~not_empty(A) then
  scitree=tree
  scitree.name="mtlb_rcond"
  repl_poss(scitree,..
      tree,A,"is not an empty matrix")
  tree=scitree
  
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
