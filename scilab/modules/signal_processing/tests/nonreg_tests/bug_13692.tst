// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13692 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13205
//
// <-- Short Description -->
// group returned an error for a discrete dynamical system with a specified
// sample time.

S = syslin(0.5, 1/%z);
[tg, fr] = group(10, S);
assert_checkequal(tg, ones(1, 10));
assert_checkequal(fr, [0:0.05:0.45]);

// An expected error
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A discrete system expected.\n"), "group", 2);
S = syslin("c", (1+%s)/(1+0.5*%s+%s^2));
assert_checkerror("group(10, S)", refMsg);

S = syslin([], (1+%s)/(1+0.5*%s+%s^2));
assert_checkerror("group(10, S)", refMsg);
