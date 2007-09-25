function [tree]=sci_type(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab type()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_type()
// V.C.

set_infos(msprintf(gettext("messages","No simple equivalent, so %s() is called."),"mtlb_type"),0);
tree.name="mtlb_type"

endfunction
