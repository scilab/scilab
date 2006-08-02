function [tree]=sci_ndims(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ndims()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)
// Conversion to double is made to have the same results for strings
if or(A.vtype==[String,Unknown]) then
  A = convert2double(A)
  tree.rhs=list(A)
end

tree.name="size"
tree=Funcall("size",1,Rhs(tree,"*"),tree.lhs)

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)
endfunction
