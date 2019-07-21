// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Nicolas Carrez
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14564 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=14564
//
// <-- Short Description -->
// fieldnames failed for empty struct, tlist & mlist

a.r = 1;
a.r = null();
assert_checkequal(fieldnames(a), []);

u = mlist("v");
assert_checkequal(fieldnames(u), []);

u = tlist("v");
assert_checkequal(fieldnames(u), []);

