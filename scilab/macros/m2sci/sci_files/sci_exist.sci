function [tree]=sci_exist(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab exist()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_exist()
// V.C.

no_equiv("Matlab exist(), mtlb_exist() is called")

set_infos("mtlb_exist() is just a partial emulation of Matlab exist() function !",2)

if rhs==1 then
  scitree=tree
  scitree.name="mtlb_exist"
  repl_poss(scitree,..
      tree,tree.rhs(1),"is a variable name")
  tree=scitree
else
  tree.name="mtlb_exist"
end
tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
