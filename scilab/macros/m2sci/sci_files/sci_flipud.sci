function [tree]=sci_flipud(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab flipud()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

treedims=tree.rhs(1).dims
treetype=tree.rhs(1).type


if typeof(tree.rhs(1))<>"variable" then
  tmp=gettempvar()
  insert(Equal(list(tmp),tree.rhs(1)))
else
  tmp=tree.rhs(1)
end

ind1=Operation(":",list(Cste("$"),Cste(-1),Cste(1)),list())
ind2=Cste(":")

tree=Operation("ext",list(tmp,ind1,ind2),tree.lhs)

tree.out(1).dims=treedims
tree.out(1).type=treetype
endfunction
