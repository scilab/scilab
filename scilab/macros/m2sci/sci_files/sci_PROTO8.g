function [tree]=sci_MFUN(tree)
// Prototype for sum prod and mean
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()
// V.C.

// B = MFUN(A)
if rhs==1 then
  A = getrhs(tree)

  // Because %b_SFUN and %C_SFUN are not defined
  A = convert2double(A)
  tree.rhs=Rhs(A)
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end

  if is_a_vector(A) | is_empty(A) then
    tree.lhs(1).dims=list(1,1)
  elseif not_a_vector(A) then
    tree.lhs(1).dims=list(1,A.dims(2))
    tree.rhs=Rhs(A,1)
  elseif not_empty(A) then
    dim = first_non_singleton(A)
    if dim>0 then
      tree.rhs=Rhs(A,dim)
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim)=1
    else
      tree.rhs=Rhs(A,Funcall("firstnonsingleton",1,list(A),list()))
      tree.lhs(1).dims=allunknown(A.dims)
    end
  else
    tree.name="mtlb_MFUN"
    tree.lhs(1).dims=allunknown(A.dims)
  end

// B = MFUN(A,dim)
else
  [A,dim] = getrhs(tree)
  
  // Because %b_SFUN and %C_SFUN are not defined
  A=convert2double(A)
  dim=convert2double(dim)
  tree.rhs=Rhs(A,dim)
  
  
  if typeof(dim)=="cste" then
    if dim.value>size(A.dims) then
      // Scilab SFUN() does not work when dim  is greater than number of dims A
      tree.name="mtlb_MFUN"
      tree.lhs(1).dims=allunknown(A.dims)
    else
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim.value)=1
    end
  else
    tree.name="mtlb_MFUN"
    tree.lhs(1).dims=allunknown(A.dims)
  end
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end
endfunction
