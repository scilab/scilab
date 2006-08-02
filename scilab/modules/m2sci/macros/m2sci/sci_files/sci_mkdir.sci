function [tree]=sci_mkdir(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab mkdir
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).dims=list(1,1);
tree.lhs(1).type=Type(Double,Real);

if lhs>=2 then
  tree.lhs(2).dims=list(1,Unknown);
  tree.lhs(2).type=Type(String,Real);
end

if lhs==3 then
  no_equiv(expression2code(tree)+" when called with three outputs.");
end

endfunction
