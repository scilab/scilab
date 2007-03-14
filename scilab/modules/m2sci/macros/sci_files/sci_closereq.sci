function [tree]=sci_closereq(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab closereq
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="delete"

tree.rhs(1)=Funcall("gcf",1,list(),list())

endfunction
