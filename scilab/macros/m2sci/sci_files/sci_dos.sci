function [tree]=sci_dos(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab dos()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==2 then
  tree.rhs(2)=null()
end

if lhs==1 then
  V=gettempvar(2)
  S=tree.lhs(1)
  if S.name=="ans" then
    S=gettempvar(1)
    tree.lhs=list(V,S)
  else
    tree.lhs=list(V,S)
  end
  insert(Equal(tree.lhs,tree))
  tree=list()
  
  if rhs==2 then
    insert(Equal(list(),Funcall("disp",1,list(S),list())),1)
  end
else
  tree.lhs=list(tree.lhs($:-1:1))

  if rhs==2 then
    insert(Equal(list(),Funcall("disp",1,list(tree.lhs(1),tree.lhs(2)),list())),1)
  end

  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(String,Real)
  
  tree.lhs(2).dims=list(1,1)
  tree.lhs(2).type=Type(Double,Real)
  
end
endfunction
