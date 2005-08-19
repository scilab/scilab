function [tree]=sci_error(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab error()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==1 then
  //Scilab error is equivalent to the Matlab one
  return
else
  tree.name="mtlb_error"
  return
end
endfunction
