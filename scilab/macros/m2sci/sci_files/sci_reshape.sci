function [tree]=sci_reshape(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab reshape()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

for k=1:rhs
  if k==1 then
    if tree.rhs(k).vtype==String then
      tree.rhs(k)=Funcall("mstr2sci",1,list(tree.rhs(k)),list(Variable("",tree.rhs(k).infer)))
    elseif tree.rhs(k).vtype==Unknown then
      scitree=tree
      scitree.rhs(k)=Funcall("mtlb_double",1,list(tree.rhs(k)),list(Variable("",tree.rhs(k).infer)))
      repl_poss(scitree,tree,X,"is not a character string matrix")
      tree=scitree
    end
  end
  if typeof(tree.rhs(k))=="cste" then
    if isempty(tree.rhs(k).value) then
      tree.rhs(k)=Cste(-1)
    end
  end
end

set_infos("WARNING: Matlab reshape() suppresses singleton higher dimension, it is not the case for matrix...",2)

tree.name="matrix"
tree.lhs(1).dims=allunknown(tree.rhs(1).dims)
tree.lhs(1).type=tree.rhs(1).type
endfunction
