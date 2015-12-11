// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug  -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13381
//
// <-- Short Description -->
// eigs failed due to a typo on varargin
//
// <-- CLI SHELL MODE -->

function y = f(x)
    A = diag(-10:10);
    y = A*x;
endfunction

[d, v] = eigs(f, 21, eye(21, 21), 7);
d2 = eigs(f, 21, eye(21, 21), 7);

assert_checkfalse(isempty(v));
