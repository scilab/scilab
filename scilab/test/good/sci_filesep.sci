function [tree]=sci_filesep(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab filesep()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="pathconvert"
tree.rhs(1)=Cste("/")

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(String,Real)
endfunction
