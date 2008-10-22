function [tree]=sci_ispc(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ispc()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree=Variable("MSDOS",Infer(list(1,1),Type(Boolean,Real)))

endfunction
