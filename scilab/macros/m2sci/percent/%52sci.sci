function [tree]=%52sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab negation
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.

A = getoperands(tree)
A = convert2double(A)
tree.operands=list(A)

tree.out(1).dims=A.dims
tree.out(1).type=Type(Boolean,Real)

endfunction
