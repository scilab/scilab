// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15811 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15811
//
// <-- Short Description -->
// comparison of containers with cell failed.

m = mlist(["m" "c"], {1});
save(TMPDIR + "/tmp.sod", "m");
oldm = m;
clear m;
load(TMPDIR + "/tmp.sod");
assert_checkequal(m, oldm);

a = {1 + 1};
b = {3 - 1};
assert_checkequal(a, b);

