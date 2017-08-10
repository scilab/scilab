
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

function [J]=fstabst(Stplant,r)
    //[J]=fstabst(Stplant,r)
    // Parametrization of all stabilizing feedbacks.
    //-- Stplant=[ Stplant11 Stplant12;
    //         Stplant21 Stplant22]
    //  (in state-space or transfer form: conversion in state-space is
    //   done for the computations)
    //-- r = size of  Stplant22 subsystem (2,2) block of Stplant
    //
    //          j =[ j11 j12;
    //               j21 j22]
    // K is a stablizing controller iff K=LFT(J,r,Q) with Q stable
    //!
    if and(typeof(Stplant)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"fstabst",1))
    end
    if Stplant.dt<>"c" then
        error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"fstabst",1))
    end
    flag=0;
    if typeof(Stplant)=="rational" then Stplant=tf2ss(Stplant),flag=1;end


    [LHS,RHS]=argn(0);
    [a,b1,b2,c1,c2,d11,d12,d21,d22]=smga(Stplant,r),
    Rd1=d12'*d12,
    R12=sqrtm(Rd1);
    Rd2=d21*d21',
    R22=sqrtm(Rd2);
    p=r(2),r=r(1);
    //-------------
    [s1,s2,t]=size(Stplant);
    [w1,k1]=rowcomp(d12),do1=w1(k1+1:s1-r,:)',
    if do1==[] then do1=0,end,
    [w2,k2]=rowcomp(d21'),do2=w2(k2+1:s2-p,:),
    if do2==[] then do2=0,end,
    //gains f and h
    //-------------
    ar1=a-b2*inv(Rd1)*d12'*c1,br1=b2*inv(Rd1)*b2',
    cr1=c1'*do1*do1'*c1,
    xc=riccati(ar1,br1,cr1,"cont"),
    f=inv(Rd1)*(b2'*xc+d12'*c1),
    ar2=(a-b1*d21'*inv(Rd2)*c2)',br2=c2'*inv(Rd2)*c2,
    cr2=b1*do2'*do2*b1',
    xo=riccati(ar2,br2,cr2,"cont"),
    h=(b1*d21'+xo*c2')*inv(Rd2),
    // J:
    //---
    aj=a-b2*f-h*c2+h*d22*f;
    bj=[h ,(b2-h*d22)*inv(R12)],
    cj=[-f;
    -inv(R22)*(c2-d22*f)],
    dj=[0*ones(p,r)            eye(p,p)*inv(R12);
    inv(R22)*eye(r,r)         d22],
    //Normalization to get inner and co-inner factors
    J=syslin("c",aj,bj,cj,dj),
    if flag==1 then J=ss2tf(J);end
endfunction
