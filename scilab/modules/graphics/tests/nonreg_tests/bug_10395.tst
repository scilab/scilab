// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10395
//
// <-- Short Description -->
//xset("clipping", [1, 2, 3, 4]) must be like xset("clipping", 1, 2, 3, 4)

ref1 = [1,2,3,4];
xset("clipping", ref1);
val1 = xget("clipping");
assert_checkequal(val1, ref1);

ref2 = [1,2,3,4];
xset("clipping", ref2(1), ref2(2), ref2(3), ref2(4));
val2 = xget("clipping");
assert_checkequal(val2, ref2);
