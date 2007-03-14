function  tree=sci_orth(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab orth()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B. 

A=getrhs(tree)
tree.lhs(1).dims=list(A.dims(1),-1)
tree.lhs(1).type=Type(Double,A.property)
endfunction