function [tree]=sci_isreal(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isreal()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_isreal and %b_isreal are not defined in Scilab
A = getrhs(tree)
A = convert2double(A)
tree.rhs=Rhs(A)

// eps set to 0
tree.rhs=Rhs(A,0)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Boolean,Real)
endfunction
