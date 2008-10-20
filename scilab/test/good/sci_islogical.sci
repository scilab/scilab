function [tree]=sci_islogical(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab islogical()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)
A=Funcall("type",1,Rhs(A),list())
mat=Operation("rc",list(Cste(4),Cste(6)),list())
tree=Operation("==",list(A,mat),tree.lhs)
tree=Funcall("or",1,list(tree),tree.out)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)
endfunction
