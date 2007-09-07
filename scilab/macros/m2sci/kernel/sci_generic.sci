function [tree]=sci_generic(tree)
// Copyright INRIA
// M2SCI function
// Generic conversion function for unknown Matlab functions
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.
if typeof(tree)=="operation"
  tree.out(1).dims=list(-1,-1)
  tree.out(1).type=Type(-1,-1)
else
  for i=1:size(tree.lhs)
    tree.lhs(i).dims=list(-1,-1)
    tree.lhs(i).type=Type(-1,-1)
  end
end
endfunction
