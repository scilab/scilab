function [tree]=sci_getenv(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab getenv()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).dims=list(1,Unknown)
tree.lhs(1).type=Type(String,Real)
endfunction
