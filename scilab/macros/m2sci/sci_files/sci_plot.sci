function [tree]=sci_plot(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab plot()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_plot()
// V.C.

tree.name="mtlb_plot"
tree.lhs(1).type=Type(Handle,Unknown)
endfunction
