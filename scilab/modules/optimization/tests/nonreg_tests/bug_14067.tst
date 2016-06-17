// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function fx = foo(x)
    fx = exp(-2*x)-x*x*x+6*x*x
endfunction

ierr = execstr('[x, fx, info] = fsolve(10, foo, [], 1.0d-12)', 'errcatch');
assert_checkequal(ierr, 0);

