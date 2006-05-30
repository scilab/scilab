function [tree]=sci_perms(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab perms()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.Belahcene.

A=getrhs(tree)
tree.rhs=Rhs(A)

if (type(tree.rhs(1).dims(1))==1 & tree.rhs(1).dims(1)==1) & (type(tree.rhs(1).dims(2))==1 & tree.rhs(1).dims(2)<>-1) then
	tree.lhs(1).dims(1)=prod(1:tree.rhs(1).dims(2))
	tree.lhs(1).dims(2)=tree.rhs(1).dims(2)
elseif (type(tree.rhs(1).dims(2))==1 & tree.rhs(1).dims(2)==1) & (type(tree.rhs(1).dims(1))==1 & tree.rhs(1).dims(1)<>-1) then
	tree.lhs(1).dims(1)=prod(1:tree.rhs(1).dims(1))
	tree.lhs(1).dims(2)=tree.rhs(1).dims(1)
else
	tree.lhs(1).dims(1)=-1
	tree.lhs(1).dims(2)=-1
end

tree.lhs(1).type=tree.rhs(1).type

endfunction
