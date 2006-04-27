function [tree]=sci_str2num(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab str2num()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

tree.name="evstr"
tree.lhs(1).type=Type(Double,Real)
endfunction