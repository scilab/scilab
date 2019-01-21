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
// <-- Non-regression test for bug 15734 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15734
//
// <-- Short Description -->
// intersect() failed with complex numbers

Zc = [%i,-%i];
assert_checkequal(intersect(Zc,Zc), [-%i %i]);
assert_checkequal(intersect(Zc,conj(Zc)), [-%i %i]);
