function [tree]=sci_etime(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab etime
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
