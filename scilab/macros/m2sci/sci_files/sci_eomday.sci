function   tree=sci_eomday(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab eomday()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B.

tree.lhs(1).type=Type(Double,Real)
tree.lhs(1).dims=list(1,1)

endfunction
