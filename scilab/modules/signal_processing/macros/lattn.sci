function [la,lb]=lattn(n,p,cov)
//[la,lb]=lattn(n,p,cov)
//macro which solves recursively on n (p being fixed)
//the following system (normal equations), i.e. identifies
//the AR part(poles) of a vector ARMA(n,p) process
//
//                        |Rp+1 Rp+2 . . . . . Rp+n  |
//                        |Rp   Rp+1 . . . . . Rp+n-1|
//                        | .   Rp   . . . . .  .    |
//                        |                          |
//    |I -A1 -A2 . . .-An|| .    .   . . . . .  .    |=0
//                        | .    .   . . . . .  .    |
//                        | .    .   . . . . .  .    |
//                        | .    .   . . . . . Rp+1  |
//                        |Rp+1-n.   . . . . . Rp    |
//
//    where {Rk;k=1,nlag} is the sequence of empirical covariances
//
//   n   : maximum order of the filter
//   p   : fixed dimension of the MA part. If p is equal to -1,
//       : the algorithm reduces to the classical Levinson recursions.
//   cov : matrix containing the Rk(d*d matrices for
//       : a d-dimensional process).It must be given the
//       : following way:
//
//                        |  R0 |
//                        |  R1 |
//                    cov=|  R2 |
//                        |  .  |
//                        |  .  |
//                        |Rnlag|
//
//   la  : list-type variable, giving the successively calculated
//       : polynomials (degree 1 to degree n),with coefficients Ak
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   if argn(2)<>3 then 
     error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),'lattn',3)) 
   end
   if type(n)<>1 then
     error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),'lattn',1))
   end
   if  size(n,'*')<>1 then
     error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),'lattn',1))
   end
   if  n<0|n<>round(n) then
     error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),'lattn',1))
   end
   if type(p)<>1 then
     error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),'lattn',2))
   end
   if  size(n,'*')<>2 then
     error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),'lattn',2))
   end
   if  p<-1|p<>round(p) then
     error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in the interval [%s, %s].\n"),'lattn',2,"-1","%inf"))
   end
   if type(cov)<>1 then
     error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),'lattn',3))
   end

   [l,d]=size(cov);
   if d>l then
     error(msprintf(gettext("%s: Wrong size for input argument #%d: A tall matrix expected.\n"),'lattn',3))
   end
   a=eye(d);b=eye(d);
   z=poly(0,'z');la=list();lb=list();
   no=p-n-1;cv=cov;
   if no<0,
     for j=1:-no,cv=[cov(j*d+1:(j+1)*d,:)';cv];end;
     p=p-no;
   end
   for j=0:n-1,
     jd=jmat(j+1,d);
     r1=jd*cv((p+1)*d+1:(p+2+j)*d,:);
     r2=jd*cv(p*d+1:(p+1+j)*d,:);
     r3=jd*cv((p-1-j)*d+1:p*d,:);
     r4=jd*cv((p-j)*d+1:(p+1)*d,:);
     c1=coeff(a);c2=coeff(b);
     k1=(c1*r1)*inv(c2*r2);
     k2=(c2*r3)*inv(c1*r4);
     a1=a-k1*z*b;
     b=-k2*a+z*b;a=a1;
     la(j+1)=a;lb(j+1)=b;
   end;
endfunction
function [la,lb]=lattp(n,p,cov)
// Author G Levey INRIA
//!
  [l,d]=size(cov);
  id=eye(d);
  [a,b]=lattn(n,0,cov);a=a(n);b=b(n);
  z=poly(0,'z');la=list();lb=list();
  jd=jmat(n+1,d);
  for j=0:p-1,
    r1=jd*cov((j+1)*d+1:(j+n+2)*d,:);
    r2=jd*cov(j*d+1:(j+n+1)*d,:);
    c1=coeff(a);c2=coeff(b);
    k=(c1*r1)*inv(c2*r2);
    hst=-inv(c1(:,n*d+1:(n+1)*d));
    r=k*hst;
    a=(id-r*z)*a-k*z*b;
    b=-hst*a;
    la(j+1)=a;
  end;
endfunction
