function [tree]=sci_isletter(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isletter()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_isletter()
// V.C.

set_infos("No simple equivalent for isletter() in Scilab so mtlb_isletter() is called",0);
set_infos("See M2SCI documentation for replacement possibilities...",1);

tree.name="mtlb_isletter"

tree.lhs(1).dims=tree.rhs(1).dims
tree.lhs(1).type=Type(Boolean,Real)

endfunction
