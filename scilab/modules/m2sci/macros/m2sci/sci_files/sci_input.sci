function [tree]=sci_input(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab input()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// input(msg)
if rhs==1 then
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Unknown,Unknown)
else
// input(msg,'s')
  tree.lhs(1).dims=list(1,Unknown)
  tree.lhs(1).type=Type(String,Real)
end
endfunction
