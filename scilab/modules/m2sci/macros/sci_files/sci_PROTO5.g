function [tree]=sci_MFUN(tree)
// Prototype for: atanh
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)
tree.rhs=Rhs(A)

set_infos(msprintf(gettext("messages","m2sci_message_74"),expression2code(A),expression2code(tree)),2)
  
tree.lhs(1).dims=A.dims
tree.lhs(1).type=Type(Double,Unknown)
endfunction
