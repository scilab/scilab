// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// Copyright (C) INRIA - 1991 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [sm,fr]=mese(x,npts);
    //<sm,fr]=mese(x [,npts]);
    //Calculate the maximum entropy spectral estimate of x
    //  x    :Input sampled data sequence
    //  npts :Optional parameter giving number of points of fr and sm
    //        (default is 256)
    //  sm   :Samples of spectral estimate on the frequency grid fr
    //  fr   :npts equally spaced frequency samples in [0,.5)
    //!

    //default evaluation

    [rhs,lhs]=argn(0);
    if lhs==1 then,
        npts=256;
    end,

    //estimate autocorrelation function of x

    Nx=length(x);
    r=convol(x,x(Nx:-1:1))
    r=r(Nx:-1:1)/Nx;

    //get solution to the Yule-Walker equations

    [ar,sigma2,rc]=lev(r);

    //compute spectrum

    ak=[1;ar];
    [sf,fr]=frmag(ak,npts);
    sm=sigma2*ones(sf)./(sf.*conj(sf));
endfunction
