// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//  <-- NOT FIXED -->

// <-- Non-regression test for bug 9529 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9529
//
// <-- Short Description -->
// assert_checkequal fails with void element in a list


voidelement_ref=list(1,,3);
voidelement=list(2,,3);
t = (voidelement_ref(2)==voidelement(2));
t = (voidelement_ref==voidelement);
assert_checkequal(voidelement, voidelement_ref);

