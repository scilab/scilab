// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 13490 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13490
//
// <-- Short Description -->
// histc now matches its help description (by default, normalizes the result),
// normalization is now done properly (in total area).
// =============================================================================

myprob = [1 2 2 3 3 3];

withNorm    = histc(3, myprob);
withoutNorm = histc(3, myprob, normalization=%f);

assert_checkalmostequal(withNorm, (1:3)/4);
assert_checkequal(withoutNorm, 1:3);
