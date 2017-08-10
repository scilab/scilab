//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14058 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14058
//
// <-- Short Description -->
// Scilab crashes with 'file("close", file())' instruction

[units,typs,nams] = file();
u = mopen(SCI+"/ACKNOWLEDGEMENTS", "rt");
u2 = mopen(SCI+"/CHANGES.md", "rt");
[units_open,typs,nams] = file();
assert_checkequal(gsort([units, u, u2], "c", "i"), units_open);
file("close", file());
[units_close,typs,nams] = file();
// Delete 1 in units because it represents bug_14058.tst file which was opened in Scilab
ind = find(units == 1);
units(ind) = [];
assert_checkequal(units_close, units);
