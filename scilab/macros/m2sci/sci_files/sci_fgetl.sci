function [tree]=sci_fgetl(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab fgetl()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.name="mgetl"
tree.rhs(2)=Cste(1)

if typeof(tree.lhs(1))=="variable" & tree.lhs(1).name=="ans" then
  out=gettempvar()
  insert(Equal(list(out),tree))
  tree=out
else
  out=tree.lhs(1)
end

// meof
meof_funcall=Funcall("meof",1,list(),list())
// meof()~=0
neq=Operation("~=",list(meof_funcall,Cste(0)),list())

// output=-1
EQ=Equal(list(out),Cste(-1)) 

if typeof(tree)=="variable" then
  insert(tlist(["ifthenelse","expression","then","elseifs","else"],neq,list(EQ),list(),list()));
else
  insert(tlist(["ifthenelse","expression","then","elseifs","else"],neq,list(EQ),list(),list()),1);
end

if typeof(tree)<>"variable" then
  tree.lhs(1).dims=list(1,Unknown)
  tree.lhs(1).type=Type(Unknown,Real)
end
endfunction
