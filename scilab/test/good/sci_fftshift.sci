function [tree]=sci_fftshift(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fftshift()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if or(tree.rhs(1).vtype==[String,Unknown]) then
  tree.name="mtlb_fftshift"
end
if rhs==2 then
  if typeof(tree.rhs(2))=="cste" then
    if tree.rhs(2).value>size(tree.rhs(1).dims) then
      tree.name="mtlb_fftshift"
    end
  else
    tree.name="mtlb_fftshift"
  end
end
tree.lhs(1).dims=tree.rhs(1).dims
tree.lhs(1).type=Type(tree.rhs(1).vtype,tree.rhs(1).property)
endfunction
