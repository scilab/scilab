function [tree]=sci_det(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab det()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X = getrhs(tree)

// %c_det and %b_det are not defined
X = convert2double(X)
tree.rhs=Rhs(X)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=X.type
endfunction
