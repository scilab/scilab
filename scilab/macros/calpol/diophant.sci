function [x,err]=diophant(p1p2,b)
//solves diophantine equation p1*x1+p2*x2=b
//with  p1p2 a polynomial vector [p1 p2]
//b polynomial
//x polynomial vector [x1;x2]
//if the equation is uncompatible err=||p1*x1+p2*x2-b||/||b||
//else err=0
//!
// Copyright INRIA
p1=p1p2(1);p2=p1p2(2)
[x,u]=bezout(p1,p2)
p1=u(2,2);p2=u(1,2)//
if degree(x)==0 then
   x=b*u(:,1)
   err=0
else
   [r,q]=pdiv(b,x)
   err=norm(coeff(b-x*q),2)/norm(coeff(b),2)
   x=q*u(:,1)
end
endfunction
