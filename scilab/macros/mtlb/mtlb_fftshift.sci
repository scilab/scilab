function y=mtlb_fftshift(x,dim)
// Copyright INRIA
// Emulation function for fftshift() Matlab function
// V.C.

rhs=argn(2)

// Y = fftshift(X)
if rhs==1 then
  if type(x)==10 then
    tmp=fftshift(mstr2sci(x))
    y=[]
    for k=1:size(tmp,1)
      y=[y;strcat(tmp(k,:))]
    end
  else
    y=fftshift(x)
  end
// Y = fftshift(X,dim)
else
  if dim<=size(size(x),"*") then
    if type(x)==10 then
      tmp=fftshift(mstr2sci(x),dim)
      y=[]
      for k=1:size(tmp,1)
	y=[y;strcat(tmp(k,:))]
      end
    else
      y=fftshift(x,dim)
    end
  else
    y=x
  end
end
endfunction
