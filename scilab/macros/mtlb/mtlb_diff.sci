function y=mtlb_diff(x,N)
[lhs,rhs]=argn()
if rhs==1 then N=1,end
if or(size(x)==1) then
  y=x(1+N:$)-x(1:$-N)
else
  y=x(1+N:$,:)-x(1:$-N,:)
end
endfunction
