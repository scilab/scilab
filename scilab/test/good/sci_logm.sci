function [tree]=sci_logm(tree)
// Generic conversion function
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab logm()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

A=getrhs(tree)
tree.rhs=Rhs(A)

tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Unknown)
if size(tree.lhs)==2
  tree.lhs(2)=null()
  set_infos(["second output argument will be ignored"],2);
end
endfunction




