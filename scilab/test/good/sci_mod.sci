function [tree]=sci_mod(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab mod()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_r_c and %b_r_b are not defined in Scilab
[X,Y] = getrhs(tree)
X = convert2double(X)
Y = convert2double(Y)
tree.rhs=Rhs(X,Y)

tree.name="pmodulo"

if is_a_scalar(X) then
  tree.lhs(1).dims=Y.dims
elseif is_a_scalar(Y) then
  tree.lhs(1).dims=X.dims
elseif X.dims==Y.dims then
  tree.lhs(1).dims=X.dims
else
  tree.lhs(1).dims=allunknown(X.dims)
end
tree.lhs(1).type=Type(Double,Real)
endfunction
