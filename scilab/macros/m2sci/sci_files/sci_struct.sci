function [tree]=sci_struct(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab struct()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="makestruct"
[tree]=sci_makestruct(tree)
return
if rhs<>0 then
  tree.lhs(1).dims=list(1,lstsize(tree.rhs)/2)
else
  tree.lhs(1).dims=list(0,0)
end
tree.lhs(1).type=Type(Struct,Unknown)
disp("sci_struct")
pause
endfunction
