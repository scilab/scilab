//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 10396 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10396
//
// <-- Short Description -->
// Display of a structure with large matrix took a long time.

a = struct();
a.toto = zeros(1,100000);
a.titi = zeros(1,100000);
tic();
a
timeSpent=toc();
assert_checktrue(timeSpent<1);
