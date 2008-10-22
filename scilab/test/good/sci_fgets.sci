function [tree]=sci_fgets(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fgets()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.


if rhs==2 then
  tree.name="mgetstr"
  tree.rhs=Rhs(tree.rhs(2),tree.rhs(1))
  if typeof(tree.rhs(1))=="cste" then
    tree.lhs(1).dims=list(1,tree.rhs(1).value)
  else
    tree.lhs(1).dims=list(1,Unknown)
  end
else
  tree.name="mgetl"
  tree.lhs(1).dims=list(1,Unknown)
  tree.rhs(2)=Cste(1)
end
tree.lhs(1).type=Type(String,Real)
endfunction
