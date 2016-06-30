// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 14306 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14306
//
// <-- Short Description -->
// Comparisons `>` and `>=` between integers of mismatching inttypes were not
// implemented.

assert_checktrue(int8(3) > int16(1));
