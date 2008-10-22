function [tree]=sci_length(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab length()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

X = getrhs(tree)
// Conversion to double is made to have the same results for strings
if or(X.vtype==[String,Unknown]) then
  X = convert2double(X)
  tree.rhs=list(X)
end

if is_a_vector(X) then
  tree=Funcall("size",1,Rhs(X,"*"),tree.lhs)
else
  tree.name="size"
  tree=Funcall("max",1,Rhs(tree),tree.lhs)
end
tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
