// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3768 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3768
//
// <-- Short Description -->
//    Odd integers x between 2^52 and 2^53 in absolute value satisfy
//    round(x)==x

x = [4503599627370499 4503599627370500 -4503599627370499 -4503599627370500];
computed = round(x);
assert_checkequal(computed,x);

