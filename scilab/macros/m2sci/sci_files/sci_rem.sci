function [tree]=sci_rem(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab rem()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_rem()
// V.C.

// %c_d_c and %b_d_b are not defined in Scilab
[X,Y] = getrhs(tree)
X = convert2double(X)
Y = convert2double(Y)
tree.rhs=Rhs(X,Y)

set_infos("See M2SCI documentation for replacement possibilities for call to mtlb_rem()",1)

tree.name="mtlb_rem"

if is_a_scalar(X) then
  tree.lhs(1).dims=Y.dims
elseif is_a_scalar(Y) then
  tree.lhs(1).dims=X.dims
elseif is_a_vector(X) then
  tree.lhs(1).dims=X.dims
elseif is_a_vector(Y) then
  tree.lhs(1).dims=Y.dims
else
  tree.lhs(1).dims=X.dims
end

tree.lhs(1).type=Type(Double,Real)

endfunction
