function [tree]=sci_rem(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab rem()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_d_c and %b_d_b are not defined in Scilab
[X,Y] = getrhs(tree)
X = convert2double(X)
Y = convert2double(Y)
tree.rhs=Rhs(X,Y)

// X./Y
drd=Operation("./",tree.rhs,list())
// fix(X./Y)
fix_funcall=Funcall("fix",1,list(drd),list())
// fix(X./Y).*Y
drm=Operation(".*",list(fix_funcall,tree.rhs(2)),list())
// X-fix(X./Y).*Y
tree=Operation("-",list(tree.rhs(1),drm),tree.lhs)

if is_a_scalar(X) then
  tree.out(1).dims=Y.dims
elseif is_a_scalar(Y) then
  tree.out(1).dims=X.dims
elseif is_a_vector(X) then
  tree.out(1).dims=X.dims
elseif is_a_vector(Y) then
  tree.out(1).dims=Y.dims
else
  tree.out(1).dims=X.dims
end

tree.out(1).type=Type(Double,Real)

endfunction
