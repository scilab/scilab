function [tree]=sci_asin(tree)
// File generated from sci_PROTO5.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab asin()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

set_infos(["If "+expression2code(A)+" is outside [-1,1]"
    "   complex part of output of "+expression2code(tree)+" will be the opposite of Matlab one."],2)
  
tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Unknown)
endfunction
