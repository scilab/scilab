function [tree]=sci_pause(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab pause
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs<1 then
  tree.name="halt"
else
  n = getrhs(tree)
  if n.vtype==String then // pause on/off
    no_equiv(expression2code(tree));
  else
    tree.rhs=Rhs(Operation("*",list(Cste(1000),n),list()))
    tree.name="xpause"
  end
end

endfunction
