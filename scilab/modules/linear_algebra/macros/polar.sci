
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ro,teta]=polar(a);
//[ro,Teta]=polar(A);
// Polar form of A
// A=Ro*exp(%i*Teta) Ro symmetric >=0 
// Teta hermitian >=0
//F.D.
//!
  [U,s,V]=svd(a);
  ro1=U*sqrt(s);
  ro=ro1*ro1';
  W=U*V';
  // A = Ro*W   (Ro sdp ; W unit)
  // W=exp(%i*Teta)
  //
  [ab,x,bs]=bdiag(W+0*%i*W);
  z=log(diag(ab));
  lw=x*diag(z)*inv(x);
  teta=-%i*lw;
endfunction
