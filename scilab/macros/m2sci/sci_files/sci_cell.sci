function [tree]=sci_cell(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cell()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==0 then
  tree.lhs(1).dims=list(0,0)
  tree.lhs(1).type=Type(Cell,Real)
  tree.lhs(1).contents=cell()
  return
end

dims=list()
for kdims=1:rhs
  if typeof(tree.rhs(kdims))=="cste" then
    dims(kdims)=tree.rhs(kdims).value
  else
    dims(kdims)=Unknown
  end
end
tree.lhs(1).dims=dims
tree.lhs(1).type=Type(Cell,Unknown)
tree.lhs(1).contents=makecell()


endfunction
