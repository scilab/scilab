function [tree]=sci_atan2(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab atan2()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_atan and %b_atan are not defined in Scilab
[Y,X]=getrhs(tree)
Y=convert2double(Y)
X=convert2double(X)
tree.rhs=Rhs(Y,X)

// Scilab atan() does not work with complex
if is_complex(Y) then
  tree.rhs(1)=Funcall("real",1,list(Y),list())
elseif ~is_real(Y) then
  tree.rhs(1)=Funcall("real",1,list(Y),list())
  repl_poss(tree.rhs(1),Y,Y,"is real")
end
if is_complex(X) then
  tree.rhs(2)=Funcall("real",1,list(X),list())
elseif ~is_real(X) then
  tree.rhs(2)=Funcall("real",1,list(X),list())
  repl_poss(tree.rhs(2),X,X,"is real")
end

tree.name="atan"

tree.lhs(1).dims=Y.dims
tree.lhs(1).type=Type(Double,Real)
endfunction
