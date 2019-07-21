// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Nicolas Carrez
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14405 -->
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14405
//
// <-- Short Description -->
// xcosPalAdd doesn't work

loadXcosLibs();
pal = xcosPal();
pal = xcosPalAddBlock(pal, "SUM_f");
pal = xcosPalAddBlock(pal, "BIGSOM_f");
assert_checktrue(xcosPalAdd(pal, "my Summation blocks"));
