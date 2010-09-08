// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [x,err]=diophant(p1p2,b)
//solves diophantine equation p1*x1+p2*x2=b
//with  p1p2 a polynomial vector [p1 p2]
//b polynomial
//x polynomial vector [x1;x2]
//if the equation is uncompatible err=||p1*x1+p2*x2-b||/||b||
//else err=0
//!
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
