function r=mtlb_cumprod(a)
// Copyright INRIA
if type(a)==4 then a=bool2s(a);end
if size(a,1)==1|size(a,2)==1 then
  r=cumprod(a)
else
  r=cumprod(a,1)
end
endfunction
