function [tree]=sci_isunix(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isunix()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree=Operation("~",list(Variable("MSDOS",Infer(list(1,1),Type(Boolean,Real)))),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
