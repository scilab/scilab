function [N,B,C]=pol2des(Ds)
// Given the polynomial matrix Ds= D_0 +D_1 s +D_2 s^2 + ... +D_k s^k,
// pol2des returns three  matrices N,B,C  (with N nilpotent) such that
// Ds = C (sN-Eye)^-1 B 
//!
// Copyright INRIA
if type(Ds)==1 then Ds=Ds+0*poly(0,'s')*Ds;end
dg=maxi(degree(Ds))+1;
[nout,nin]=size(Ds);
[Sl]=markp2ss(coeff(Ds),dg,nout,nin);
N=Sl(2);B=-Sl(3);C=Sl(4)
endfunction
