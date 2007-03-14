function [tree]=sci_delete(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab delete()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X = getrhs(tree)

if X.vtype==String then
  tree.name="mdelete"
elseif X.vtype<>Handle then
  tree.name="mtlb_delete"
end

endfunction
