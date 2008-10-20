function [tree]=sci_cd(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cd()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// cd
if rhs<=0 then
  tree.lhs(1).dims=list(1,Unknown)
  tree.lhs(1).type=Type(String,Real)
// cd(dirname)
else
end
endfunction
