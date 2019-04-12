// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15580 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15580
//
// <-- Short Description -->
// [det([]) was wrong and ] det(sparse([],[]) yielded an error

esp = sparse([],[]);
assert_checkequal(det(esp),1);
[e,m]= det(esp);
assert_checkequal([e m], [0 1]);

assert_checkequal(det([]), 1);
