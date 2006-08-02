function [tree]=sci_MFUN(tree)
// Prototype for cumsum and cumprod
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
  
  dim=first_non_singleton(A)

  tree.lhs(1).dims=A.dims
  
  if dim>0 then
    tree.rhs=Rhs(A,dim)
  elseif dim==-1 then
    tree.rhs=Rhs(A,Funcall("firstnonsingleton",1,list(A),list()))
  end
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
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
      if typeof(A)=="funcall" then
	tree.lhs(1).infer=A.infer
	A.lhs=tree.lhs
      elseif typeof(A)=="operation" then
	tree.lhs(1).infer=A.infer
	A.out=tree.lhs
      end
      tree=A;
      set_infos("Scilab SFUN() does not work when dim input argument is greater than number of dims of first rhs...",1)
      return
    end
  end
  
  tree.name="mtlb_MFUN"
  tree.lhs(1).dims=A.dims
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end
endfunction
