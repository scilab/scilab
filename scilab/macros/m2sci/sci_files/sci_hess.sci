function [tree]=sci_hess(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab hess()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs(A)

// H = hess(A)
if lhs==1 then
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
// [P,H] = hess(A)
else
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(A.vtype,Unknown) // Real if A<>%i
  tree.lhs(2).dims=A.dims
  tree.lhs(2).type=A.type
  
end
endfunction
