// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8031 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8031
//
// <-- Short Description -->
// Fixed cdfgam error message about the bound of the result.
// =============================================================================

shape = 0.5;
rate = 1;
p = 1.12837916D-150;
q = 1;
x = 0;

refMsg = msprintf(_("%s: Answer appears to be lower than lowest search bound %g\n"), "cdfgam", 1e-300);
assert_checkerror("shape1 = cdfgam(""Shape"", shape, p, q, x);", refMsg);
