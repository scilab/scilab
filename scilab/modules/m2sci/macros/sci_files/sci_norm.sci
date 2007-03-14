function [tree]=sci_norm(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab norm()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_norm()
// V.C.

// n = norm(A)
if rhs==1 then
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A)
// n = norm(A,p)
else
  [A,p] = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A,p)
end

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
