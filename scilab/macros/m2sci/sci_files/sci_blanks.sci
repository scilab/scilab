function [tree]=sci_blanks(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab blanks()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

A=getrhs(tree)
A=convert2double(A)

if ~is_a_scalar(A) & typeof(A)<>"variable" then
  n=gettempvar()
  insert(Equal(list(n),A))
  B=Operation("ext",list(n,Cste(1)),list())
  repl_poss(B,..
      A,A,"is a scalar")
elseif ~is_a_scalar(A) then
  B=Operation("ext",list(A,Cste(1)),list())
  repl_poss(B,..
      A,A,"is a scalar")
else
  B=A
end

tree.name="part"
ones_funcall=Funcall("ones",1,list(Cste(1),B),list())
tree.rhs=list(Cste(" "),ones_funcall)

if typeof(A)=="cste" then
  tree.lhs(1).dims=list(1,A.value(1))
else
  tree.lhs(1).dims=list(1,Unknown)
end
tree.lhs(1).type=Type(String,Real)
endfunction
