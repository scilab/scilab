// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

//<-- ENGLISH IMPOSED -->

rand("seed", 0);
n  = 2;
A1 = rand(n, n);
A2 = rand(n, n);
Xs = diag(1:n);
Q1 = -(A1'*Xs+Xs*A1+0.1*eye());
Q2 = -(A2'*Xs+Xs*A2+0.2*eye());

function [LME, LMI, OBJ] = evalf(Xlist)
    X   = Xlist(1)
    LME = X-diag(diag(X))
    LMI = list(-(A1'*X+X*A1+Q1), -(A2'*X+X*A2+Q2))
    OBJ = -sum(diag(X))
endfunction

X = lmisolver(list(zeros(A1)), evalf);
X = X(1);

refX = [1.0635042 0; 0 2.0784841];
refY = 0;
refZ = list([0.0731600 0.7080179; 0.7080179 0.7186999], [0.1154910 0.5345239; 0.5345239 1.4843684]);
refC = -1.0635042;

[Y, Z, c] = evalf(X);

assert_checkalmostequal(X, refX, [], 1d-6);
assert_checkalmostequal(Y, refY, [], 1d-6);
assert_checkalmostequal(Z(1), refZ(1), [], 1d-6);
assert_checkalmostequal(Z(2), refZ(2), [], 1d-6);
assert_checkalmostequal(c, refC, [], 1d-6);
