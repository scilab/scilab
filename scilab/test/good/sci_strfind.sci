function [tree]=sci_strfind(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab strfind()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_strfind()
// V.C.

[A,B] = getrhs(tree)

if A.vtype==String & B.vtype==String then
  tree.name="strindex"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Double,Real)
else
  tree.name="mtlb_strfind"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
