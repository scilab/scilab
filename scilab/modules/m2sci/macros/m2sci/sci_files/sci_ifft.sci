function [tree]=sci_ifft(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab ifft()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_ifft()
// V.C.

tree.name="fft"

// Y = ifft(X)
if rhs==1 then
  X = getrhs(tree)
  // %c_ifft and %b_ifft are not defined
  X = convert2double(X)
  tree.rhs=Rhs(X,1)
  
  if size(X.dims)==2 then
    if is_a_scalar(X) | is_a_vector(X) then
      tree.rhs=Rhs(X,1)
    elseif not_a_vector(X) then
      if X.dims(2)<>Unknown then
	tree.rhs=Rhs(X,1,X.dims(1),1)
      else
	tmp=Funcall("size",1,list(X,1),list())
	tree.rhs=Rhs(X,1,tmp,1)
      end
    else
      tree.name="mtlb_ifft"
      tree.rhs=Rhs(X)
    end
  else
    tree.rhs=Rhs(X,1,Funcall("firstnonsingleton",1,list(X,"num"),list()),1)
  end
  tree.lhs(1).dims=X.dims
  tree.lhs(1).type=Type(Double,Unknown)

// Y = ifft(X,n)
elseif rhs==2 then
  [X,n] = getrhs(tree)
  X = convert2double(X)
  n = convert2double(n)
  tree.rhs=Rhs(X,n)
  
  tree.name="mtlb_ifft"
  
  if is_empty(X) 
  tree.lhs(1).dims=list(0,0)
  else
  tree.lhs(1).dims=allunknown(X.dims)
  end
  tree.lhs(1).type=Type(Double,Unknown)
  
// Three input args
elseif rhs==3 then
  [X,n,dim] = getrhs(tree)
  X = convert2double(X)
  n = convert2double(n)
  dim = convert2double(dim)
  tree.rhs=Rhs(X,n,dim)
  
  tree.name="mtlb_ifft"
  
  tree.lhs(1).dims=allunknown(X.dims)
  tree.lhs(1).type=Type(Double,Unknown)
 
end

endfunction
