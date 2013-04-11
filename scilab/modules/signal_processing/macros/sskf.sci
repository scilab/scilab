// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xe, x]=sskf(y,f,h,q,r,x0)
//<xe>=sskf(y,f,h,q,r,x0)
//steady-state kalman filter
// y   :data in form [y0,y1,...,yn], yk a column vector
// f   :system matrix dim(NxN)
// h   :observations matrix dim(NxM)
// q   :dynamics noise matrix dim(NxN)
// r   :observations noise matrix dim(MxM)
//
// xe  :estimated state
//!

//get steady-state Kalman gain
 
 x=ricc(f',h'/r*h,q,'disc') // steady state err cov
 k=x*h'/(h*x*h'+r)

 // estimate state
 kfd=(eye(f)-k*h)*f;
 [xe]=ltitr(kfd,k,y,x0);
endfunction
