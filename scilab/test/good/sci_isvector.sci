function [tree]=sci_isvector(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isvector()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if or(tree.rhs(1).vtype==[String,Unknown]) then
  tree.rhs(1)=convert2double(tree.rhs(1))
end

rowsize=Funcall("size",1,Rhs(tree.rhs(1),1),list())
colsize=Funcall("size",1,Rhs(tree.rhs(1),2),list())

rowsizeeq=Operation("==",list(rowsize,Cste(1)),list())
colsizeeq=Operation("==",list(colsize,Cste(1)),list())

tree=Operation("|",list(rowsizeeq,colsizeeq),tree.lhs)

tree.out(1).dims=list(1,1)
tree.out(1).type=Type(Boolean,Real)

endfunction
