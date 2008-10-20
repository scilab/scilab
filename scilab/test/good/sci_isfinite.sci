function [tree]=sci_isfinite(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isfinite()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_isfinite and %b_isfinite are not defined in Scilab
A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs(A)

tree=Funcall("abs",1,list(A),tree.lhs)
tree=Operation("<",list(tree,Cste(%inf)),tree.lhs)

tree.out(1).dims=A.dims
tree.out(1).type=Type(Boolean,Real)
endfunction
