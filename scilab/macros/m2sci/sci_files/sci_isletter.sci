function [tree]=sci_isletter(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isletter()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_isletter()
// V.C.

tree.name="mtlb_isletter"

tree.lhs(1).dims=tree.rhs(1).dims
tree.lhs(1).type=Type(Boolean,Real)

endfunction
