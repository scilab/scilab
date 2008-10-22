function [tree]=sci_eval(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab eval()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_eval()
// V.C.

if lhs==1 then
  // eval(expression)
  if rhs==1 then
    tree.name="mtlb_eval"
  // eval(expression,catch_expr)
  else
    tree.name="mtlb_eval"
  end
else
  tree.name="mtlb_eval"

  // lhs names
  LHS=[]
  for k=1:lhs
    LHS=[LHS,tree.lhs(k).name]
  end
  LHS="["+strcat(LHS,",")+"]"
  if typeof(tree.rhs(1))=="cste" then
    tree.rhs=Rhs(LHS+" = "+tree.rhs(1).value)
  else
    tree.rhs=Rhs(LHS+" = "+expression2code(tree.rhs(1)))
  end
  tree.lhs=list(Variable("ans",Infer()))
end
endfunction
