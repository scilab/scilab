// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13482 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13482
//
// <-- Short Description -->
// pfss changed the variable of the rational fractions
//
// <-- CLI SHELL MODE -->

f = %z / (2-%z) / (%z+1);
d = pfss(f);

// f and d must have the same variable name
assert_checkequal(varn(f), varn(d(1)));
assert_checkequal(varn(f), varn(d(2)));
