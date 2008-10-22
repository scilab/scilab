function [tree]=sci_real(tree)
// Generic conversion function
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab real()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Real)
endfunction
