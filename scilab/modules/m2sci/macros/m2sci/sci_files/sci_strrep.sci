function [tree]=sci_strrep(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab strrep()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_strrep()
// V.C.

[s1,s2,s3] = getrhs(tree)

if s1.vtype==String & s2.vtype==String & s3.vtype==String then
  tree.name="strsubst"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(String,Real)
else
  tree.name="mtlb_strrep"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Unknown,Unknown)
end
endfunction
