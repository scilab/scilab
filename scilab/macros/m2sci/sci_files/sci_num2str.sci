function [tree]=sci_num2str(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab num2str()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==1 then
  set_infos("string output can be different from Matlab num2str output",2)
  tree.name="string"
else
  if tree.rhs(2).vtype==String then
    tree.name="msprintf"
    tree.rhs=Rhs(tree.rhs(2),tree.rhs(1))
  elseif tree.rhs(2).vtype<>Unknown then
    no_equiv(expression2code(tree))
    set_infos("See msprintf for solutions",1);
  else
    tree.name="mtlb_num2str"
  end
end
endfunction
