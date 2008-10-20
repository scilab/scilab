function [tree]=sci_logical(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab logical()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_logical()
// V.C.

X = getrhs(tree)
// Conversion to double is made to have the same results for strings
if or(X.vtype==[String,Unknown]) then
  X = convert2double(X)
  tree.rhs=list(X)
end

if is_empty(X) then
  set_infos(expression2code(X)+" is an empty matrix, so result is set to []",0);
  tree=Cste([])
elseif not_empty(X) then
  if X.vtype==Boolean then
    if typeof(X)=="operation" then
      X.out(1)=tree.lhs(1)
    elseif typeof(X)=="funcall" then
      X.lhs(1)=tree.lhs(1)
    end
    
    tree=X
  else
    tree=Operation("<>",list(X,Cste(0)),tree.lhs)
  
    tree.out(1).dims=X.dims
    tree.out(1).type=Type(Boolean,Real)
  end
else
  tree.name="mtlb_logical"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Boolean,Real)
end
endfunction
