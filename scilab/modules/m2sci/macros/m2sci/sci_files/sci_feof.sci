function [tree]=sci_feof(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab feof()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

fid=getrhs(tree)

tree.name="meof"

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
