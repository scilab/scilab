function [tree]=sci_svd(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab svd()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// svd(X)
if rhs==1 then
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A)
// svd(A,0)
else
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A,"e")
end

tree.lhs(1).dims=list(Unknown,1)
tree.lhs(1).type=Type(Double,Real)
if lhs>1 then
  tree.lhs(2).dims=list(Unknown,Unknown)
  tree.lhs(2).type=A.type
end
if lhs>2 then
  tree.lhs(3).dims=list(Unknown,Unknown)
  tree.lhs(3).type=Type(Double,Unknown)
end

endfunction
