function [tree]=sci_abs(tree)
// Generic conversion function
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab abs()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Real)
endfunction
