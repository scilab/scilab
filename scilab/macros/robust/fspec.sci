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
// Copyright INRIA
g1=g(1);
flag='ss';if g1(1)=='r' then flag='tf';g=tf2ss(g);end
      [r1,r,d]=dtsi(g),[a,b,c]=r(2:4),
      ari=a-b*inv(d)*c,
      rri=b*inv(d)*b',qri=-c'*inv(d)*c,
      x=riccati(ari,rri,qri,'c'),
      id=sqrtm(d),
      gm=syslin('c',a,b,inv(id)*(c+b'*x),id),
      gm=minss(gm)
if flag=='tf' then gm=ss2tf(gm);end;
endfunction
