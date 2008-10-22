function [tree]=sci_generic(tree)
// Copyright INRIA
// M2SCI function
// Generic conversion function for unknown Matlab functions
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

for i=1:size(tree.lhs)
  tree.lhs(i).dims=list(-1,-1)
  tree.lhs(i).type=Type(-1,-1)
end

endfunction
