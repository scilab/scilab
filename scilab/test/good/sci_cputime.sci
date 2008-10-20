function [tree]=sci_cputime(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cputime()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="timer"

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
