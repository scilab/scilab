function  [tree]=sci_cellstr(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cellstr()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

A=getrhs(tree)
tree.rhs=Rhs(A)
Aknowndim=%t // Aknowndim is a boolean, true if all dimennsions of A are known
tree.lhs(1).dims=list()
for i=1:size(A.dims) 
  if A.dims(i)==-1 then
    Aknowndim=%f
    break
  end
end
if ~Aknowndim then
  tree.lhs(1).dims(1)=-1
  tree.lhs(1).dims(2)=1
else
  tree.lhs(1).dims(1)=A.dims(1)
  tree.lhs(1).dims(2)=1
end
tree.lhs(1).type=Type(Cell,Unknown)
endfunction
