function [tree]=sci_error(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab error()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs>1 then
  no_equiv(expression2code(tree))
end
endfunction
