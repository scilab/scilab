function [tree]=sci_cumprod(tree)
// File generated from sci_PROTO2.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cumprod()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_cumprod()
// V.C.

// B = cumprod(A)
if rhs==1 then
  A = getrhs(tree)

  // Because %b_cumprod and %C_cumprod are not defined
  A = convert2double(A)
  tree.rhs=Rhs(A,"m")
  
  tree.lhs(1).dims=A.dims
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end    
// B = cumprod(A,dim)
else
  [A,dim] = getrhs(tree)
  
  // Because %b_cumprod and %C_cumprod are not defined
  A=convert2double(A)
  dim=convert2double(dim)
  tree.rhs=Rhs(A,dim)
  
  if typeof(dim)=="cste" then
    if dim.value>size(A.dims) then
      if typeof(A)=="funcall" then
	tree.lhs(1).infer=A.infer
	A.lhs=tree.lhs
      elseif typeof(A)=="operation" then
	tree.lhs(1).infer=A.infer
	A.out=tree.lhs
      end
      tree=A;
      set_infos("Scilab cumprod() does not work when dim input argument is greater than number of dims of first rhs...",1)
      return
    end
  end
  
  tree.name="mtlb_cumprod"
  tree.lhs(1).dims=A.dims
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end
endfunction
