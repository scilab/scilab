//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- Non-regression test for bug 10305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10305
//
// <-- Short Description -->
// Compare lists with empty items is impossible

l1 = list(1,,2);
l2=l1;
assert_checkequal(l1==l2, [%t %f %t]);
