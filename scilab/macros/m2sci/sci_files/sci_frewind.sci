function [tree]=sci_frewind(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab frewind()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="mseek"
tree.rhs=Rhs(0,tree.rhs(1))

endfunction
