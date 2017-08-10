// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14543 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14543
//
// <-- Short Description -->
//    == and <> were not overloaded for rational

r = 1/%z - 1/%z;
assert_checktrue(r == 0);
assert_checktrue(r <> 1);
assert_checkfalse(r <> 0);

r = (2/%s)/(1/%s);
assert_checktrue(r == 2);
assert_checkfalse(r <> 2);
