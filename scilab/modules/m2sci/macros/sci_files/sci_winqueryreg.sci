function [tree]=sci_winqueryreg(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab winqueryreg
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// Matlab returns a Cell of strings or a int32
// Scilab returns a matrix of strings or a int32
tree.lhs(1).dims=list(Unknown,1);
tree.lhs(1).type=Type(Unknown,Real);

endfunction
