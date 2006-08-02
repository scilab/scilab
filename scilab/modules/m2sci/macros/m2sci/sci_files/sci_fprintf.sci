function [tree]=sci_fprintf(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fprintf()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_fprintf()
// V.C.

set_infos(["No simple equivalent, so mtlb_fprintf() is called"],0);

tree.name="mtlb_fprintf"

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
