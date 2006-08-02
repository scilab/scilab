function [tree]=sci_full(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab full()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_full()
// V.C.

S = getrhs(tree)

if S.vtype==String then
  tree.name="mtlb_full"
  tree.lhs(1).dims=S.dims
  tree.lhs(1).type=S.type
elseif S.vtype==Boolean then
  S = convert2double(S)
  tree.rhs=Rhs(S)
  tree.lhs(1).dims=S.dims
  tree.lhs(1).type=Type(Boolean,Real)
elseif S.vtype==Double then
  tree.lhs(1).dims=S.dims
  tree.lhs(1).type=S.type
elseif S.vtype==Sparse then
  tree.lhs(1).dims=S.dims
  tree.lhs(1).type=Type(Double,S.property)
else
  tree.name="mtlb_full"
  tree.lhs(1).dims=S.dims
  tree.lhs(1).type=S.type
end

endfunction
