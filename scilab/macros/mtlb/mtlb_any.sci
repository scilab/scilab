function r=mtlb_any(a)
// Copyright INRIA
if size(a,1)==1|size(a,2)==1 then
  r=or(a)
else
  r=or(a,1)
end
endfunction
