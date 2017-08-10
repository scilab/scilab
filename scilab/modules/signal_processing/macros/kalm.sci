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

    if argn(2) <> 8 then
        error(sprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "kalm", 8));
    end
    k=p0*h'*(h*p0*h'+r)**(-1);
    p=(eye(p0)-k*h)*p0;
    p1=f*p*f'+g*q*g';
    x=x0+k*(y-h*x0);
    x1=f*x;
endfunction
