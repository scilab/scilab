function [tree]=sci_setstr(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab setstr()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_setstr()
// V.C.

X = getrhs(tree)

if X.vtype==String then
  set_infos([
      "In "+expression2code(tree)+" "+expression2code(X)+" is a String"
      "So result is set to "+expression2code(X)],0)
  // Because of String concatenation
  if typeof(X)=="operation" then
    X.out(1)=tree.lhs(1)
  elseif typeof(X)=="funcall" then
    X.lhs(1)=tree.lhs(1)
  end
  tree=X
elseif X.vtype==Unknown then
  scitree=tree
  scitree.name="mtlb_setstr"
  repl1=tree
  
  repl_poss(scitree,..
      Funcall("ascii",1,list(X),list()),X,"is a Double",..
      X,X,"is a String")

  tree=scitree
  tree.lhs(1).dims=X.dims
  tree.lhs(1).type=Type(String,Real)
else
  tree.name="ascii"
  if X.dims(1)==1 | is_a_scalar(X) then // Row vector or scalar
    tree.lhs(1).dims=X.dims
    tree.lhs(1).type=Type(String,Real)
  else
    scitree=tree
    scitree.name="mtlb_setstr"
    repl_poss(scitree,..
	Funcall("ascii",1,list(X),list()),X,"is a row vector or a scalar")
    tree=scitree
    tree.lhs(1).dims=X.dims
    tree.lhs(1).type=Type(String,Real)
  end
end
endfunction
