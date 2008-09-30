// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [K]=%k(m)
//K=%k(m)
//Calculates Jacobi's complete elliptic integral
//of the first kind:
//  K = integral from 0 to 1 of
//      [(1-t**2)(1-m*t**2)]**(-1/2)
//m is allowed to be a vector
//Ref :Abramowitz and Stegun page 598
//  m :Parameter used in calculating the elliptic
//    :integral where 0<m<1.
//  K :Value of the elliptic integral from 0 to 1
//    :on the real axis.
//
//!

   [n1,n2]=size(m);
   un=ones(n1,n2);
   a=un;
   b=sqrt(un-m);
   c=sqrt(m);
   while maxi(abs(c)) > %eps,
      an=0.5*(a+b);
      bn=sqrt(a.*b);
      cn=0.5*(a-b);
      a=an;
      b=bn;
      c=cn;
   end,
   K=%pi*un./(2*a);
endfunction
