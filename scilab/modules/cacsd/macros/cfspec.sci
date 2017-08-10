// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [gm]=cfspec(g)
    //[gm]=cfspec(g) computes a co-spectral factorization:
    //        g = gm*gtild(gm)
    //with stable gm and gm^-1  ( gm^-1 = invsyslin(gm) ).
    //-- g: syslin list defining the linear system g
    //-- gm:
    //Assumptions:
    //- g is invertible ( inv(D) exists ),
    //- g and g^1 (invsyslin(g)) have no poles on the imaginary axis.
    //- gtild(g) = g.
    //  (poles and zeros of g are symmetric wrt imaginary axis))
    //- g(+oo) = D is positive definite.
    //!

    if type(g)==1 then gm=chol(g),return,end,
    gm=fspec(g'),
    gm=gm'
endfunction
