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
  tree.name="mtlb_rcond"
  
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
