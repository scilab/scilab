// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7666 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7666
//
// <-- Short Description -->
// Inconstencies between and/or and &/|

assert_checkequal(or([0.6 -0.9]), 0.6 | -0.9);
assert_checkequal(or([0.6 -1.1]), 0.6 | -1.1);
assert_checkequal(and([0.6 -0.7]), 0.6 & -0.7);
assert_checkequal(and([0.6 -1.1]), 0.6 & -1.1);
assert_checkequal(and([2.1 -1.1]), 2.1 & -1.1);