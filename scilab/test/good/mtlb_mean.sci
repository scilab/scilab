function m = mtlb_mean(x) ;
// Copyright INRIA
if or(size(x)==1) then
  m=mean(x)
else
  m=mean(x,'r')
end
endfunction
