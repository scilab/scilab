function [tree]=sci_linspace(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab linspace()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_linspace()
// V.C.

// Boolean value: true if one rhs is a String
str=%F

A=tree.rhs(1)
B=tree.rhs(2)

// %c_linspace and %b_linspace are not defined
if or(A.vtype==[String,Boolean]) then
  if A.vtype==String then
    str=%T
  end
  A=convert2double(A)
end
if or(B.vtype==[String,Boolean]) then
  if B.vtype==String then
    str=%T
  end
  B=convert2double(B)
end

// y = linspace(A,B)
if rhs==2 then
  tree.rhs=Rhs(A,B)
  if A.vtype==Unknown | B.vtype==Unknown then
    tree.name="mtlb_linspace"
  elseif str then
    tree=Funcall("ascii",1,list(tree),tree.lhs)
  end
  tree.lhs(1).dims=list(1,100);
// y = linspace(A,B,n)
else
  n=tree.rhs(3)
  if or(n.vtype<>Double) then
    n=convert2double(n)
  end
  tree.rhs=Rhs(A,B,n)
  if A.vtype==Unknown | B.vtype==Unknown | n.vtype==Unknown then
    tree.name="mtlb_linspace"
  elseif str then
    tree=Funcall("ascii",1,list(tree),tree.lhs)
  end
  if typeof(n)=="cste" then
    if isempty(n.value) then
      tree.lhs(1).dims=list(1,1)
    else
      tree.lhs(1).dims=list(1,n.value)
    end
  else
    tree.lhs(1).dims=list(1,Unknown)
  end
end

endfunction
