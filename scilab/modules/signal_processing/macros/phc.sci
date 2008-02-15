function [h,f,g]=phc(hk,d,r)
//[h,f,g]=phc(hk,d,r)
//macro which computes the matrices h, f, g by the principal hankel
//component approximation method, from the hankel matrix built
//from the covariance sequence of a stochastic process.
//   hk  : hankel matrix
//   d   : dimension of the observation
//   r   : desired dimension of the state vector
//       : for the approximated model
//   h,f,g  : relevant matrices of the state-space model
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   [p,q]=size(hk);
   [u,s,v]=svd(hk);
   s=diag(sqrt(diag(s(1:r,1:r))));
   obs=u(:,1:r)*s;
   g=s*v(:,1:r)';
   g=g(:,1:d);
   ofl=obs(d+1:p,:);opr=obs(1:p-d,:);
   f=opr\ofl;
   h=obs(1:d,:);
endfunction
