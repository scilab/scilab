function [d]=detr(h)
//[d]=detr(h)  computes de determinant of a polynomial or 
//rational function matrix h using Leverrier's method
//!
// Copyright INRIA
h1=h(1);
if type(h)< 3 then
   [m,n]=size(h);
   if m<>n then error(20),end
   f=eye(n,n);
   for k=1:n-1,
       b=h*f,
       d=-sum(diag(b))/k
       f=b+eye(n,n)*d,
   end
   d=-sum(diag(h*f))/n;
              else //
   if h1(1)<> 'r' then error(44),end
   [m,n]=size(h(2));
   if m<>n then error(20),end
   f=eye(n,n);
   for k=1:n-1,
       b=h*f,
       d=0;for l=1:n,d=d+b(l,l),end,d=-d/k;
       f=b+eye(n,n)*d,
   end
   b=h*f;d=0;for l=1:n,d=d+b(l,l),end;d=-d/n;
end
if 2*int(n/2)<>n then d=-d;end
endfunction
