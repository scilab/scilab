function y=mtlb_triu(x,k)
// Copyright INRIA
// Emulation function for triu() Matlab function
// V.C.
rhs=argn(2);
if rhs==2 then
  if type(x)==10 then
    x=asciimat(x);
    tmp=triu(x,k);
    y=[]
    for l=1:size(tmp,1)
      tmp1=mstr2sci(ascii(tmp(l,:)));
      tmp1(find(tmp1==ascii(0)))=ascii(0)
      tmp1=strcat(tmp1);
      y=[y;tmp1]
    end
  elseif type(x)==4 then
    y=triu(bool2s(x),k)
  else
    y=triu(x,k);
  end
else
  if type(x)==10 then
    x=asciimat(x);
    tmp=triu(x);
    y=[]
    for l=1:size(tmp,1)
      tmp1=mstr2sci(ascii(tmp(l,:)));
      tmp1(find(tmp1==ascii(0)))=ascii(0)
      tmp1=strcat(tmp1);
      y=[y;tmp1]
    end
  elseif type(x)==4 then
    y=triu(bool2s(x))
  else
    y=triu(x);
  end
end
endfunction
