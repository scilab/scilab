function [tree]=sci_isfield(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isfield()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

st=tree.rhs(1)
f=tree.rhs(2)
if and(st.vtype<>[Struct,Unknown]) then
  tree=Cste(%F)
elseif st.vtype==Unknown then
  tree.name="mtlb_tree"
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Boolean,Real)  
else
  // allf=getfield(1,st);tf=or(allf(3:$)==f);
  allf=gettempvar()
  insert(Equal(list(allf),Funcall("getfield",1,Rhs(1,st),list())))
  
  imp=Operation(":",list(Cste(3),Cste("$")),list())
  ext=Operation("ext",list(allf,imp),list())
  eq=Operation("==",list(ext,f),list())
  
  tree=Funcall("or",1,Rhs(eq),tree.lhs)
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Boolean,Real)  
end
endfunction
