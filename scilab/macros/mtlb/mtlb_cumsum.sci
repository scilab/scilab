function r=mtlb_cumsum(a)
// Copyright INRIA
if type(a)==4 then a=bool2s(a);end
if size(a,1)==1|size(a,2)==1 then
  r=cumsum(a)
else
  r=cumsum(a,1)
end
