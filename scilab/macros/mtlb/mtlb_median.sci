function r=mtlb_median(a)
// Copyright INRIA
if or(size(a)==1) then
  r=median(a)
else
  r=median(a,'r')
end
endfunction
