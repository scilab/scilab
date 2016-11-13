
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

function [gm]=fspec(g)
    // computes a spectral factorization:
    //        g = gtild(gm)*gm
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

    if and(typeof(g)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"fspec",1))
    end
    if g.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"fspec",1))
    end
    flag=0;
    if typeof(g)=="rational" then g=tf2ss(g),flag=1;end
    [r1,r,d]=dtsi(g),[a,b,c]=r(2:4),
    ari=a-b*inv(d)*c,
    rri=b*inv(d)*b',qri=-c'*inv(d)*c,
    x=riccati(ari,rri,qri,"c"),
    id=sqrtm(d),
    gm=syslin("c",a,b,inv(id)*(c+b'*x),id),
    gm=minss(gm)
    if flag==1 then gm=ss2tf(gm);end;
endfunction
