// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 11802 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11802
//
// <-- Short Description -->
// mrfit function redefines the diff function (making nyquist function fail when called just after).

a = [10 20 30 40 50];
b = [1 2 3 4 5];
tt = mrfit(a,b,2);

assert_checktrue(execstr("nyquist(tt)", "errcatch")==0);