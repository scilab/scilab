function [tree]=%t2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab transpose
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_t()
// V.C.

A = getoperands(tree)

tree.out(1).dims=list(A.dims(2),A.dims(1))
tree.out(1).type=A.type

// Scilab and Matlab transposition do not work in the same way for strings
if or(A.vtype==[String,Unknown]) then
  tree=Funcall("mtlb_t",1,Rhs(A),tree.out)
end
  
endfunction

