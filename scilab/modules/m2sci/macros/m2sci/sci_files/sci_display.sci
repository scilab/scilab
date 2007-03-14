function [tree]=sci_display(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab display()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

set_infos(gettext("messages","m2sci_message_86"),2);
tree.name="disp"

endfunction
