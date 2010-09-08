// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [p,s,t,l,rt,tt]=srfaur(h,f,g,r0,n,p,s,t,l)
//square-root algorithm for the algebraic Riccati equation.
//
//   h,f,g  : convenient matrices of the state-space model.
//   r0     : E(yk*yk').
//   n      : number of iterations.
//
//   p      : estimate of the solution after n iterations.
//   s,t,l  : intermediate matrices for
//          : successive iterations;
//   rt,tt  : gain matrices of the filter model after n iterations.
//
//          : p,s,t,l may be given as input if more than one recursion
//          : is desired (evaluation of intermediate values of p, e.g.).
//!

[lhs,rhs]=argn(0);
[d,m]=size(h);
if rhs==5,
  //initializations
  r0=.5*(r0+r0');
  s=sqrtm(r0);
  t=(g/s)';s=s';
  l=-%i*t;
  p=l'*l;
else,
  if rhs<>9 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),'srfaur',9));
  end;
end;
//recursion
for j=1:n,
  a=[s t;l*h' l*f'];
  [q,r]=qr(a);
  s=r(1:d,1:d);
  t=r(1:d,d+1:d+m);
  l=r(d+1:2*d,d+1:d+m);
  p=p+l'*l;
end;
//gain matrices of the filter.
rt=r0-h*p*h';
tt=(g-f*p*h')*inv(rt);
endfunction
