function [tree]=sci_isequal(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isequal()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if tree.rhs(1).vtype==Struct then
  set_infos("isequal requires fields are in the same order for structs to be equal",2);
end

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)
endfunction
