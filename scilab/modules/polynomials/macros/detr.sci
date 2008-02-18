// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [d]=detr(h)
//[d]=detr(h)  computes de determinant of a polynomial or
//rational function matrix h using Leverrier's method
//!
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
