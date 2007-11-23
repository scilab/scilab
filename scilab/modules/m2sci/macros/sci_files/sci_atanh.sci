function [tree]=sci_atanh(tree)
// File generated from sci_PROTO5.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab atanh()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

set_infos(msprintf(gettext("If %s is outside [-1,1]\n   complex part of output of %s will be the opposite of Matlab one."),expression2code(A),expression2code(tree)),2)
  
tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Unknown)
endfunction
