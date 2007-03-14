function [tree]=sci_factorial(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab factorial()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B.

A=getrhs(tree)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Real)
endfunction