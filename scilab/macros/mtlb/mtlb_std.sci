function m = mtlb_std(x,flag) ;
// Copyright INRIA
[lhs,rhs]=argn()
if rhs<2 then flag=0,end
if or(size(x)==1) then
  m=st_deviation(x)
  n=size(x,'*')
else
  m=st_deviation(x,'r')
  n=size(x,1)
end

if flag==1 then
  m=m*sqrt(1-1/n)
end
endfunction
