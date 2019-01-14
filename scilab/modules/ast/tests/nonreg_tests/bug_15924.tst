// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15924 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15924
//
// <-- Short Description -->
// sparse inequality to 0 yields a wrong result

assert_checkequal(sparse([1 0;0 -1])<=0,sparse([%f %t;%t %t]));
assert_checkequal(sparse([1 0;0 -1])>=0,sparse([%t %t;%t %f]));

