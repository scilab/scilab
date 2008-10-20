function [tree]=sci_ishold(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ishold()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

gca_funcall=Funcall("gca",1,list(),list())
get_funcall=Funcall("get",1,Rhs(gca_funcall,"auto_clear"),list())
tree=Operation("==",list(get_funcall,Cste("off")),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
