// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [P,r]=macglov(Sl)
    //[P,r]=macglov(Sl)
    //Standard plant for the Glover-McFarlane problem:
    // for this problem mu_optimal = 1-hankel_norm([N,M])
    // with [N,M]=LCF(Sl) (Normalized coprime factorization)
    // gama_optimal = 1/sqrt(mu_optimal)
    //!
    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),..
        "macglov",1))
    end

    if and(typeof(Sl)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"macglov",1))
    end
    if Sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"macglov",1))
    end
    flag=0;
    if typeof(Sl)=="rational" then Sl=tf2ss(Sl),flag=1;end
    [A,B,C,D]=abcd(Sl);[n,nb]=size(B);[nc,n]=size(C);
    r=size(D);
    [Z,H]=gfare(Sl);
    R1=eye()+D*D';
    R12=sqrtm(R1);
    Ap=A;
    Bp=[-H*R12,B];
    Cp=[C;0*ones(nb,n);C];
    Dp=[R12,0*C*B;
    0*ones(nb,nc),eye(nb,nb);
    R12,D];
    P=syslin("c",Ap,Bp,Cp,Dp);
    if flag==1 then P=ss2tf(P);end
endfunction
