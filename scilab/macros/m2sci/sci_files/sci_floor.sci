function [tree]=sci_floor(tree)
// File generated from sci_PROTO6.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab floor()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
if is_real(A) then
  tree.lhs(1).type=Type(Double,Real)
else
  tree.lhs(1).type=Type(Double,Unknown)
end
endfunction
