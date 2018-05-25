// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15263 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15263
//
// <-- Short Description -->
// Assignments in a structures array often overwrote some untargeted elements

clear s
s.r = 1.2345;
s(2,1).r = 2.345;
s(1,2).r = %i;
assert_checkequal(s(2,2).r, []);
s(2,2).r = 1-%i;
assert_checkequal(s(1,2).r, %i);

