function [tree]=sci_isscalar(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isscalar()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

lgth_funcall=Funcall("length",1,tree.rhs,list())
sum_funcall=Funcall("sum",1,Rhs(lgth_funcall),list())
tree=Operation("==",list(sum_funcall,Cste(1)),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
