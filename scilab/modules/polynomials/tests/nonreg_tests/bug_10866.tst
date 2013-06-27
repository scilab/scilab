// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10866 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10866
//
// <-- Short Description -->
//    the det function was not equivalent to detr function for rational matrices.
// =============================================================================

x = poly(0,'x');
M=[-1, 0, 0, 0;...
    0,-1, 0, 0;...
    0, 0,-1, 0;...
    0, 0, 0,-1;...
    1, 0, 1, 0;...
    0, 1, 0, 1];

G=(1+x)/(1+x^2);
B=1/(1+x^2);      

Y=[-B, G,  0,   0; ...
   -G,-B,  0,   0; ...
    0, 0,-B/5, G/5;...
    0, 0,-G/5,-B/5];

K = M*Y*M';
res = det(K);
resR = detr(K);
assert_checkequal(numer(res), 0*x);
assert_checkequal(degree(res.den), degree(resR.den));
