function [f,e]=log2(x)
[lhs,rhs]=argn(0)
if lhs==1 then
  f=log(x)/log(2)
else
  k = find(x<>0);
  e(k) = ceil(log(abs(x(k)))/log(2));
  f(k) = x(k)./(2^e(k));
  k = find(f>=1);
  f(k) = 0.5
  e(k) = e(k)+1
end
 
