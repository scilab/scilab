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
// Copyright INRIA
Stplant1=Stplant(1);
flag='ss';if Stplant1(1)=='r' then flag='tf';Stplant=tf2ss(Stplant);end
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
    xc=riccati(ar1,br1,cr1,'cont'),
    f=inv(Rd1)*(b2'*xc+d12'*c1),
    ar2=(a-b1*d21'*inv(Rd2)*c2)',br2=c2'*inv(Rd2)*c2,
    cr2=b1*do2'*do2*b1',
    xo=riccati(ar2,br2,cr2,'cont'),
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
    J=syslin('c',aj,bj,cj,dj),
    if flag=='tf' then J=ss2tf(J);end
endfunction
