function [tree]=sci_toc(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab toc
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

no_equiv(expression2code(tree))

set_infos("See timer() for solutions");

endfunction
