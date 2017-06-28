// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15117 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15117
//
// <-- Short Description -->
// Imbricated optim/leastsq/lsqrsolve/fsolve calls either crashed or returned errors


xmes = [-0.25  -0.5  -0.75  -1  -1.25  -1.5]';
bb = [1:6]';

// Exemple bidon fsolve
function xres = fonc(a, b)
    deff("y = fsol1(x)","y = a*x+b")
    xres = fsolve(0, fsol1)
endfunction

// Pour appel leastsq
function e = evalopti(X)
    aa = X
    xres = []
    for i=1:6
        xres = [xres; fonc(aa,bb(i))]
    end
    e = xmes-xres
endfunction

// Pour appel lsqrsolve
function e = evaloptim(X, m)
    e = evalopti(X)
endfunction

// Même problème réécrit pour appel datafit
Y = xmes';
X = bb';
Z = [Y; X];
// Pour appel datafit
function e = G(p, z)
    y = z(1, :)
    x = z(2, :)
    e = y-fonc(p, x)
endfunction


/////////// Lancement optimisation

x0 = 100;

// leastsq
iprint = 2;
[fopt, xopt] = leastsq(iprint, evalopti, x0);
assert_checkequal([fopt xopt], [0 4]);

// lsqrsolve
[xsol, v] = lsqrsolve(x0, evaloptim, 6);
assert_checkequal(xsol, 4);
assert_checkequal(v, zeros(6, 1));

// datafit
p0 = x0;
[p, err] = datafit(G, Z, p0);
assert_checkalmostequal([p err], [4 0], [], 1e-9);
