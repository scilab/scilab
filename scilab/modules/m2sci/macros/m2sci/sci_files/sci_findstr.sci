function [tree]=sci_findstr(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab findstr()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_findstr()
// V.C.

set_infos("No equivalent for findstr() in Scilab so mtlb_findstr() is called",0);

tree.name="mtlb_findstr"

tree.lhs(1).dims=list(Unknown,Unknown)
tree.lhs(1).type=Type(Double,Real)
endfunction
