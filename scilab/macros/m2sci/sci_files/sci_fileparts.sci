function [tree]=sci_fileparts(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fileparts()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

for klhs=1:lhs
  tree.lhs(k).dims=list(Unknown,Unknown)
  tree.lhs(k).type=Type(String,Real)
end

// Add trailing / or \ to path
pathconvert_funcall=Funcall("pathconvert",1,Rhs("/"),list())
if lhs==1 then
  tree=Operation("+",list(tree,pathconvert_funcall),tree.lhs)
else
  add=Operation("+",list(tree.lhs(1),pathconvert_funcall),list())
  insert(Equal(list(tree.lhs(1)),add),1)
end

if lhs==4 then
  insert(Equal(list(tree.lhs(4)),Cste("")),1)
  tree.lhs(4)=null()
end
endfunction
