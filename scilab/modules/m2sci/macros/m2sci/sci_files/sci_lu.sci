function [tree]=sci_lu(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab lu()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// lu(X)
if rhs==1 then
  X = getrhs(tree)
  X = convert2double(X)
  tree.rhs=Rhs(X)
  // Y = lu(X)
  if lhs==1 then
    if tree.lhs(1).name=="ans" then
      no_equiv(expression2code(tree))
    else
      no_equiv(tree.lhs(1).name+" = "+expression2code(tree))
    end
  // [L,U] = lu(X)
  elseif lhs==2 then
    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(1).type=Type(X.vtype,Real)
    tree.lhs(2).dims=list(Unknown,Unknown)
    tree.lhs(2).type=X.type
  // [L,U,P] = lu(X)
  elseif lhs==3 then
    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(1).type=Type(X.vtype,Real)
    tree.lhs(2).dims=list(Unknown,Unknown)
    tree.lhs(2).type=X.type
    tree.lhs(3).dims=list(Unknown,Unknown)
    tree.lhs(3).type=Type(X.vtype,Real)
  // [L,U,P,Q] = lu(X)
  else
    no_equiv("["+tree.lhs(1).name+","+tree.lhs(2).name+","+tree.lhs(3).name+"] = "+expression2code(tree))
  end
// lu(X,thresh)
else
    no_equiv(expression2code(tree))
end
endfunction
