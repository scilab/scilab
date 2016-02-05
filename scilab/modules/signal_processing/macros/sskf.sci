// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

    x=ricc(f',h'/r*h,q,"disc") // steady state err cov
    k=x*h'/(h*x*h'+r)

    // estimate state
    kfd=(eye(f)-k*h)*f;
    [xe]=ltitr(kfd,k,y,x0);
endfunction
