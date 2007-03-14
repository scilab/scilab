function [tree]=sci_type(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab type()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_type()
// V.C.

set_infos(msprintf(gettext("messages","m2sci_message_102"),"mtlb_type"),0);
tree.name="mtlb_type"

endfunction
