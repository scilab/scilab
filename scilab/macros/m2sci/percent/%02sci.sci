function [tree]=%02sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab dot transpose
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_0()
// V.C.

A = getoperands(tree)

// Scilab and Matlab transposition do not work in the same way for strings
if or(A.vtype==[String,Unknown]) then
  tree=Funcall("mtlb_0",1,Rhs(A),tree.out)
  tree.lhs(1).dims=list(A.dims(2),A.dims(1))
  tree.lhs(1).type=A.type
else
  if A.vtype==Boolean then
    tree.operands=list(Funcall("bool2s",1,list(A),list()))
  end
tree.out(1).dims=list(A.dims(2),A.dims(1))
tree.out(1).type=A.type
end
  
endfunction

