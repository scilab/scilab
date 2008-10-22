function [tree]=sci_isdir(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isdir()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)
endfunction
