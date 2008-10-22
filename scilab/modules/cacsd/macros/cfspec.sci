// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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
