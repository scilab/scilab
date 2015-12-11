// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

//<-- ENGLISH IMPOSED -->

// Constants
rand("seed", 0);
n  = 2;
A1 = rand(n, n);
A2 = rand(n, n);
Xs = diag(1:n);
Q1 = -(A1'*Xs+Xs*A1+0.1*eye());
Q2 = -(A2'*Xs+Xs*A2+0.2*eye());

// Redefining messagebox() to avoid popup
prot = funcprot();
funcprot(0);
function messagebox(msg, msg_title, info)
endfunction
funcprot(prot);

lmitool("prob", "X", "A1, A2"); // Create prob.sci
deletefile "prob.sci";
copyfile("SCI/modules/optimization/demos/prob_bak.sci", "prob.sci"); // Replace prob.sci by prob_bak.sci
exec("prob.sci", -1);
X = prob(A1, A2); // Run optimization

refX = [1.0635042 0; 0 2.0784841];
refY = 0;
refZ = list([0.0731600 0.7080179; 0.7080179 0.7186999], [0.1154910 0.5345239; 0.5345239 1.4843684]);
refC = -1.0635042;

[Y, Z, c] = prob_eval(X); // Check evaluaton function value at the point found

assert_checkalmostequal(X, refX, [], 1d-6);
assert_checkalmostequal(Y, refY, [], 1d-6);
assert_checkalmostequal(Z(1), refZ(1), [], 1d-6);
assert_checkalmostequal(Z(2), refZ(2), [], 1d-6);
assert_checkalmostequal(c, refC, [], 1d-6);

deletefile "prob.sci"; // Delete created file
