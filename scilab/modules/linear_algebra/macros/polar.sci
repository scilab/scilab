
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
