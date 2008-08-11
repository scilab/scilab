function [la,sig,lb]=levin(n,cov)
//[la,sig,lb]=levin(n,cov)
//macro which solves recursively on n
//the following Toeplitz system (normal equations)
//
//
//		|R1   R2   . . . Rn  |
//		|R0   R1   . . . Rn-1|
//		|R-1  R0   . . . Rn-2|
//		| .    .   . . .  .  |
// |I -A1 . -An|| .    .   . . .  .  |
//		| .    .   . . .  .  |
//		| .    .   . . .  .  |
//		|R2-n R3-n . . . R1  |
//		|R1-n R2-n . . . R0  |
//
//  where {Rk;k=1,nlag} is the sequence of nlag empirical covariances
//
//  n   : maximum order of the filter
//  cov : matrix containing the Rk (d*d matrices for a
//      : d-dimensional process). It must be given the
//      : following way:
//
//		|  R0  |
//		|  R1  |
//		|  R2  |
//		|  .   |
//		|  .   |
//		| Rnlag|
//
//  la  : list-type variable, giving the successively calculated
//      : polynomials (degree 1 to degree n), with coefficients Ak
//  sig : list-type variable, giving the successive
//      : mean-square errors.
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - G. Le Vey
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   [lhs,rhs]=argn(0);
   if rhs<>2 then
     error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),'levin',2));
   end
   [l,d]=size(cov);
   if d>l then
     error(msprintf(gettext("%s: Wrong size for input argument #%d: A tall matrix expected.\n"),'levin',2));
   end
//
//   Initializations
//
   a=eye(d,d);b=a;
   z=poly(0,'z');la=list();lb=list();sig=list();
   p=n+1;cv=cov;
   for j=1:p,cv=[cov(j*d+1:(j+1)*d,:)';cv];end;
   for j=0:n-1,
     //
     //   Bloc permutation matrix
     //
     jd=jmat(j+1,d);
     //
     //   Levinson algorithm
     //
     r1=jd*cv((p+1)*d+1:(p+2+j)*d,:);
     r2=jd*cv(p*d+1:(p+1+j)*d,:);
     r3=jd*cv((p-1-j)*d+1:p*d,:);
     r4=jd*cv((p-j)*d+1:(p+1)*d,:);
     c1=coeff(a);c2=coeff(b);
     sig1=c1*r4;gam1=c2*r2;
     k1=(c1*r1)*inv(gam1);
     k2=(c2*r3)*inv(sig1);
     a1=a-k1*z*b;
     b=-k2*a+z*b;a=a1;
     la(j+1)=a;lb(j+1)=b;sig(j+1)=sig1;
   end;
endfunction
