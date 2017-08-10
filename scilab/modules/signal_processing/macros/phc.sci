// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
