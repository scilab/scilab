function [tree]=%i2sci_g(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion in matrices (called by %i2sci())
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.
from=tree.operands($)
to=tree.operands(1)
ind=tree.operands(2)

newtree=Funcall("mtlb_i",1,Rhs(to,ind,from),list(to))
insert(Equal(list(to),newtree))
tree=list()
endfunction

  
 