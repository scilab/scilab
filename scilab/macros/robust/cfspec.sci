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
// Copyright INRIA
if type(g)==1 then gm=chol(g),return,end,
gm=fspec(g'),
gm=gm'
endfunction
