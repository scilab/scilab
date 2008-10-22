function [tree]=sci_isnan(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isnan()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_isnan and %b_isnan are not defined in Scilab
A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Boolean,Real)
endfunction
