function [tree]=sci_prod(tree)
// File generated from sci_PROTO8.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab prod()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_prod()
// V.C.

// B = prod(A)
if rhs==1 then
  A = getrhs(tree)

  // Because %b_prod and %C_prod are not defined
  A = convert2double(A)
  tree.rhs=Rhs(A,"m")
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end

  if is_a_vector(A) | is_empty(A) then
    tree.lhs(1).dims=list(1,1)
  elseif not_a_vector(A) then
    tree.lhs(1).dims=list(1,A.dims(2))
  elseif not_empty(A) then
    dim = first_non_singleton(A)
    if dim>0 then
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim)=1
    else
      tree.lhs(1).dims=allunknown(A.dims)
    end
  else
    tree.lhs(1).dims=allunknown(A.dims)
  end

// B = prod(A,dim)
else
  [A,dim] = getrhs(tree)
  
  // Because %b_prod and %C_prod are not defined
  A=convert2double(A)
  dim=convert2double(dim)
  tree.rhs=Rhs(A,dim)
  
  
  if typeof(dim)=="cste" then
    if dim.value>size(A.dims) then
      // Scilab prod() does not work when dim  is greater than number of dims A
      tree.name="mtlb_prod"
      tree.lhs(1).dims=allunknown(A.dims)
    else
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim.value)=1
    end
  else
    tree.name="mtlb_prod"
    tree.lhs(1).dims=allunknown(A.dims)
  end
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end
endfunction
