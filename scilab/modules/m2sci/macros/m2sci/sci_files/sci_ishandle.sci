function [tree]=sci_ishandle(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ishandle()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)
A=Funcall("type",1,Rhs(A),list())
tree=Operation("==",list(A,Cste(9)),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
