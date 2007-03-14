function tree=sci_bitand(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab bitand()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

tree.lhs(1).dims=tree.rhs(1).dims
tree.lhs(1).type=Type(Double,Real)
endfunction
