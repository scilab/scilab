// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15395
//
// <-- Short Description -->
// ones(2,3,2) / %z yielded an error

o = ones(2,3,2);
assert_checkequal(o/%z, rlist(o, o*%z));
