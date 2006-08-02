function y=mtlb_diag(x,dim)
// Copyright INRIA
// Emulation function for diag() Matlab function
// V.C.

rhs=argn(2)

// B = diag(A)
if rhs==1 then
  if type(x)==10 then
    x=mstr2sci(x);
    tmp=diag(x)
    tmp(find(tmp==" "))=ascii(0)
    y=[]
    for k=1:size(tmp,1)
      y=[y;strcat(tmp(k,:))]
    end
  elseif or(type(x)==[4,6]) then  
    y=diag(bool2s(x))
  else
    y=diag(x)
  end
// B = diag(A,k)
else
  if type(x)==10 then
    x=asciimat(x);
    x=diag(x,dim)
    y=[]
    for k=1:size(x,1)
      tmp=mstr2sci(ascii(x(k,:)))
      tmp(find(tmp==ascii(0)))=ascii(0)
      y=[y;strcat(tmp)]
    end
  elseif or(type(x)==[4,6]) then  
    y=diag(bool2s(x),dim)
  else
    y=diag(x,dim)
  end
end
endfunction
