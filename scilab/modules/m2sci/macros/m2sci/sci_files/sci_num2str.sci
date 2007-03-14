function [tree]=sci_num2str(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab num2str()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==1 then
  set_infos(gettext("messages","m2sci_message_116"),2)
  tree.name="string"
else
  if tree.rhs(2).vtype==String then
    tree.name="msprintf"
    tree.rhs=Rhs(tree.rhs(2),tree.rhs(1))
  elseif tree.rhs(2).vtype<>Unknown then
    no_equiv(expression2code(tree))
    set_infos(gettext("messages","m2sci_message_117"),1);
  else
    tree.name="mtlb_num2str"
  end
end
endfunction
