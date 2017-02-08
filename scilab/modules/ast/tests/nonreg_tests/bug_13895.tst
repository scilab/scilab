// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13895 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13895
//
// <-- Short Description -->
//    p.a.h = 1; p.b.h = 3; p(:).h  CRASH

p.a.h = 1;
p.b.h = 3;

refMsg = msprintf("Unknown field : %s.\n", "h");
assert_checkerror("p(:).h", refMsg);
