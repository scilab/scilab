function [tree]=sci_schur(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab schur()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)
// %c_schur and %b_schur are not defined
A = convert2double(A)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=A.type

if lhs==2 then
  tree.lhs(2).dims=A.dims
  tree.lhs(2).type=A.type
end
endfunction
