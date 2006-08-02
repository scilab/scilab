function tree=sci_date(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab date()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).dims=list(1,11)
tree.lhs(1).type=Type(String,Real)
endfunction
