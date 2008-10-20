function y=mtlb_tril(x,k)
// Copyright INRIA
// Emulation function for tril() Matlab function
// V.C.
rhs=argn(2);
if rhs==2 then
  if type(x)==10 then
    x=asciimat(x);
    tmp=tril(x,k);
    y=[]
    for l=1:size(tmp,1)
      tmp1=mstr2sci(ascii(tmp(l,:)));
      tmp1(find(tmp1==ascii(0)))=ascii(0)
      tmp1=strcat(tmp1);
      y=[y;tmp1]
    end
  elseif type(x)==4 then
    y=tril(bool2s(x),k)
  else
    y=tril(x,k);
  end
else
  if type(x)==10 then
    x=asciimat(x);
    tmp=tril(x);
    y=[]
    for l=1:size(tmp,1)
      tmp1=mstr2sci(ascii(tmp(l,:)));
      tmp1(find(tmp1==ascii(0)))=ascii(0)
      tmp1=strcat(tmp1);
      y=[y;tmp1]
    end
  elseif type(x)==4 then
    y=tril(bool2s(x))
  else
    y=tril(x);
  end
end
endfunction
