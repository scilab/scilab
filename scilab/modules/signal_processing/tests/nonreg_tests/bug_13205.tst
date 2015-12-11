// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13205 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13205
//
// <-- Short Description -->
// group accepted continuous transfer functions.

refMsg = msprintf(_("%s: Wrong type for input argument #%d: A discrete system expected.\n"), "group", 2);
s = poly(0, "s");
h = s/(s-.5);
assert_checkerror("group(100, h)", refMsg);

// Working example
h = ss2tf(cls2dls(tf2ss(syslin("c",1/s)),0.1));
[tg, fr] = group(100, h);
