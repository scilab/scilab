function r=mtlb_sum(a)
// Copyright INRIA
if type(a)==4 then a=bool2s(a);end
if size(a,1)==1|size(a,2)==1 then
  r=sum(a)
else
  r=sum(a,1)
end
endfunction
