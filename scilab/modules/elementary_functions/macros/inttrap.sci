function v = inttrap(x,y)
//inttrap  Trapezoidal numerical integration.
//v = inttrap(x,y) computes the integral of y with respect to x using
//trapezoidal integration.  x and y must be vectors of the same dimension
//v = inttrap(y) computes the trapezoidal integral of y assuming unit
//spacing between the data points. 
// Copyright INRIA

[lhs,rhs]=argn(0)
if rhs<2 then
  y=x;
  v=sum(y(1:$-1) + y(2:$))/2;
else
  if size(x,'*')<>size(y,'*') then
    error('input vectors must have the same dimension');
  end
  x=x(:);y=y(:);
  v=(x(2:$)-x(1:$-1))'*(y(1:$-1) + y(2:$))/2;
end
endfunction
