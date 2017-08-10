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

function [N,M]=lcf(Sl)
    //Compute Normalized coprime factorization of a linear dynamic system
    //%Syntax and parameters description
    //     [N,M]=lcf(Sl)
    //
    //  SL  : linear dynamic system given in state space or transfer function.
    //       see syslin
    //  N,M : is realization of Sl: Sl = M^-1 N
    //!

    if and(typeof(Sl)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lcf",1))
    end
    if Sl.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"lcf",1))
    end
    flag=0;
    if typeof(Sl)=="rational" then Sl=tf2ss(Sl),flag=1;end

    [A,B,C,D]=Sl(2:5);[nw,nb]=size(B);[nc,nw]=size(C);
    R=eye()+D*D';
    [Z,H]=gfare(Sl);
    Ar=A+H*C;
    Bn=B+H*D;Bm=H;
    Rm12=inv(sqrtm(R));
    Cr=Rm12*C;Dn=Rm12*D;Dm=Rm12;
    N=syslin("c",Ar,Bn,Cr,Dn);
    M=syslin("c",Ar,Bm,Cr,Dm);
    if flag==1 then N=ss2tf(N);M=ss2tf(M);end
endfunction
