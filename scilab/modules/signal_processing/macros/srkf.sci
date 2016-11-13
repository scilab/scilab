// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x1,p1]=srkf(y,x0,p0,f,h,q,r)
    //square root kalman filter algorithm
    //Input to the macro is:
    // f,h    :current system matrices
    // q,r    :covariance matrices of dynamics
    //        :and observation noise
    // x0,p0  :state estimate and error variance
    //        :at t=0 based on data up to t=-1
    // y      :current observation
    //
    //Output from the macro is:
    // x1,p1  :updated estimate and error covariance
    //        :at t=1 based on data up to t=0
    //!

    n=max(size(x0));
    p=max(size(y));

    j=[chol(r)',0*r];
    g=[0*q,chol(q)'];

    mat=[h*p0,j;f*p0,g];
    [q,tmat]=qr(mat');
    tmat=tmat';
    p1=tmat(p+1:p+n,p+1:p+n);
    k=tmat(p+1:p+n,1:p);
    re=tmat(1:p,1:p);

    epsilon=y-h*x0;
    x1=f*x0+k*(re**(-1))*epsilon;
endfunction
