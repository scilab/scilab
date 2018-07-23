// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15635 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15635
//
// <-- Short Description -->
// delip(1,4) terminates with neither output nor error (regression)

errMsg=sprintf(_('%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n'), "delip", 2, -1, 1);
assert_checkerror('delip(1,4)',errMsg);
