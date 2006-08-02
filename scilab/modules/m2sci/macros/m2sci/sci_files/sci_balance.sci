function [tree]=sci_balance(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab balance()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A = getrhs(tree)

// %c_balance and %b_balance are not defined
A=convert2double(A)
tree.rhs=Rhs(A)

tree.name="balanc"

// B=balance(A)
if lhs==1 then
  if tree.lhs(1).name=="ans" then
    B=gettempvar(1)
    T=gettempvar(2)
    insert(Equal(list(B,T),tree))
    tree=B
  else
    T=gettempvar(2)
    tree.lhs=list(tree.lhs(1),T)
  end
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
  tree.lhs(2).dims=A.dims
  tree.lhs(2).type=Type(Double,Real)
// [T,B] = balance(A)
else
  tree.lhs=list(tree.lhs(2),tree.lhs(1))
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Double,Real)
  tree.lhs(2).dims=A.dims
  tree.lhs(2).type=A.type
end

endfunction
