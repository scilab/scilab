function [tree]=sci_chol(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab chol()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X = getrhs(tree)

if X.vtype==Sparse then
  tree.name="spchol"
else
  tree.name="chol"
end

// %c_chol and %b_chol are not defined
X=convert2double(X)
tree.rhs=Rhs(X)

// R = chol(X)
if lhs==1 then
  tree.lhs(1).dims=X.dims
  tree.lhs(1).type=X.type
  // [R,p] = chol(X)
else
  no_equiv("Call to chol() with 2 outputs")
end

endfunction
