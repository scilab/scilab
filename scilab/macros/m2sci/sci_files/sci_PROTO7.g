function [tree]=sci_MFUN(tree)
// Prototype for: triu tril
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()
// V.C.

// L = MFUN(X,k)
if rhs==2 then
  [X,k] = getrhs(tree)
  // Convert k to double
  k=convert2double(k)
  // Imaginary part ok k is ignored in Matlab
  if is_complex(k) then
    k = Funcall("real",1,list(k),list(Variable("",k.infer)))
  elseif ~is_real(k) then
    newk = Funcall("real",1,list(k),list(Variable("",k.infer)))
    repl_poss(newk,k,k,"is real");
    k=newk
  end
  tree.rhs=Rhs(X,k)
// L = MFUN(X)
else
  X = getrhs(tree)
end

// Common part
if or(X.vtype==[String,Unknown]) then
  tree.name="mtlb_MFUN"
else
  if X.vtype==Boolean then
    X=Funcall("bool2s",1,Rhs(X),list(Variable("",X.infer)))
    tree.rhs(1)=X
  end
end

if isempty(strindex(tree.name,"triu")) then
  tree.lhs(1).type=X.type
else
  tree.lhs(1).type=Type(X.vtype,Unknown)
end
tree.lhs(1).dims=X.dims
  
endfunction
