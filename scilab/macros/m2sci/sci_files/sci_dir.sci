function [tree]=sci_dir(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab dir()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if tree.lhs(1).name=="ans" then
  tree.name="mtlb_dir"
else
  no_equiv(expression2code(tree))
  set_infos("See listfiles() for solutions...");
end

endfunction
