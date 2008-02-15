function [x1,p1,x,p]=kalm(y,x0,p0,f,g,h,q,r)
//[x1,p1,x,p]=kalm(y,x0,p0,f,g,h,q,r)
//macro which gives the Kalman update and error variance
//Input to the macro is:
// f,g,h  :current system matrices
// q,r    :covariance matrices of dynamics and observation noise
// x0,p0  :state estimate and error variance at t=0 based
//        :on data up to t=-1
// y      :current observation
//
//Output from the macro is:
// x1,p1  :updated estimate and error covariance at t=1
//        :based on data up to t=0
// x,p    :updated estimate and error covariance at t=0
//        :based on data up to t=0
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   k=p0*h'*(h*p0*h'+r)**(-1);
   p=(eye(p0)-k*h)*p0;
   p1=f*p*f'+g*q*g';
   x=x0+k*(y-h*x0);
   x1=f*x;
endfunction
