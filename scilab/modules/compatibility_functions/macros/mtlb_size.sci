function varargout=mtlb_size(X,dim)
// Copyright INRIA
// Emulation function for Matlab size()
// V.C.

varargout=list()

[lhs,rhs]=argn()

// d=size(X,dim)
if rhs==2 then
  if dim<=size(size(X),2) then
    varargout(1)=size(X,dim)
  else
    varargout(1)=1
  end
// [d1,...,dn]=size(X)  
else
  // if n < size(size(X),2)
  if lhs<size(size(X),2) then
    // d1,...,dn-1
    for k=1:lhs
      varargout(k)=size(X,k)
    end
    // dn is equal to the product of remaining dimensions 
    last=1
    for k=lhs+1:size(size(X),2)
      last=last*size(X,k)
    end
    varargout(lhs)=last
  else
    
    for k=1:size(size(X),2)
      varargout(k)=size(X,k)
    end
    
    // if n > size(size(X),2)
    if lhs>size(size(X),2)
      for k=size(size(X),2)+1:lhs
	varargout(k)=1
      end
    end
  end
end
endfunction
