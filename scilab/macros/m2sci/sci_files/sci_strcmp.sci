function [tree]=sci_strcmp(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab strcmp()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_strcmp()
// V.C.

[A,B] = getrhs(tree)

if A.vtype==String & B.vtype==String then
  tree = Operation("==",list(A,B),tree.lhs)
  tree.out(1).dims=list(1,1)
  tree.out(1).type=Type(Boolean,Real)
elseif A.vtype<>Unknown & B.vtype<>Unknown then
  set_infos("At least one argument is not a character string matrix in call of strcmp() so result is set to 0",0)
  tree = Cste(%F)
else
  scitree=tree
  scitree.name="mtlb_strcmp"
  repl1=Operation("==",list(A,B),tree.lhs)
  
  repl_poss(scitree,..
      repl1,list(A,B),"are character string matrices",..
      Cste(0),list(A,B),"are not character string matrices")
  
  tree=scitree
  tree.out(1).dims=list(1,1)
  tree.out(1).type=Type(Boolean,Real)
end
endfunction
