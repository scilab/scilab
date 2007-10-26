function v = intsplin(x,s)
//splin  numerical integration.
//v = intsplin(x,s) computes the integral of y with respect to x using
//splin interpolation and integration.  
//x and y must be vectors of the same dimension
//
//v = intsplin(s) computes the integral of y assuming unit
//spacing between the data points. 
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<2 then
  s=x;
  s=s(:);
  d=splin((1:size(s,'*'))',s);
  v=sum((d(1:$-1)-d(2:$))/12 + (s(1:$-1)+s(2:$))/2);
else
  if size(x,'*')<>size(s,'*') then
    error('input vectors must have the same dimension');
  end
end
x=x(:);s=s(:);
d=splin(x,s);
h=x(2:$)-x(1:$-1);
v=sum((h.*(d(1:$-1)-d(2:$))/12 + (s(1:$-1)+s(2:$))/2).*h);
endfunction
