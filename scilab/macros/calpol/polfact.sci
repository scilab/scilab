function [f]=polfact(p)
// Minmal factors of p
// f=polfact(p)
//
// p : polynomila
// f : vector [f0 f1 ... fn] such that p=prod(f) 
//     - f0  constant
//     - fi polynomial
//!
//
// Copyright INRIA
if size(p,'*')<>1 then error('polynomial argument required!'),end
if norm(imag(coeff(p)))<>0 then error('real case only!'),end
n=degree(p);f=coeff(p,n);
if n==0 then return,end
var=varn(p);
r=roots(p);[s,k]=sort(abs(r));r=r(k)
k=1;
while k<=n do,
  if imag(r(k))<>0 then
    f=[f,real(poly(r(k:k+1),var))]
    k=k+2
  else
    f=[f,poly(r(k),var)]
    k=k+1
  end
end
endfunction
