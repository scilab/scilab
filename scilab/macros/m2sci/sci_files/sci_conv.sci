function [tree]=sci_conv(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab conv()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_conv()
// V.C.

[A,B]=getrhs(tree)
A=convert2double(A)
B=convert2double(B)
tree.rhs=list(A,B)

if and([A.dims(1),B.dims(1)]==1) | and([A.dims(2),B.dims(2)]>1) then // A and B row vectors
  conv=tree
  conv.name="convol"
  tree.name="clean"
  tree.rhs=list(conv)
  tree.lhs(1).type=Type(Double,Unknown)
elseif A.dims(1)>1 | A.dims(2)==1 | B.dims(1)>1 | B.dims(2)==1 then // A and/or B is a column vector
  conv=tree
  conv.name="convol"
  tree.name="clean"
  tree.rhs=list(conv)
  tree=Operation(".''",list(tree),tree.lhs)
  tree.out(1).type=Type(Double,Unknown)
else
  tree.name="mtlb_conv"
  tree.lhs(1).type=Type(Double,Unknown)
end

endfunction
