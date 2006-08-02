function [tree]=sci_display(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab display()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

set_infos("Verify that overloading functions have been written for inputs in display()",2);
tree.name="disp"

endfunction
