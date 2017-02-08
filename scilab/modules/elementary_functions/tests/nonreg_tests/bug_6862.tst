// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 6862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6862
//
// <-- Short Description -->
// vectorfind(M,V,dir)
// * The number of mandatory entries is not checked.
// * typeof(M)==typeof(V) is not checked.

// Check that at least 2 inputs are needed
msg = msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "vectorfind", 2, 5);
assert_checkerror("vectorfind([2 2])", msg);

// Check that "r" option is added by default
assert_checkequal(vectorfind([2 2], [2 2]), vectorfind([2 2], [2 2], "r"));

// Check that an error message is returned if first and second input arguments have not the same type
msg = msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same type expected.\n"), "vectorfind", 1, 2);
assert_checkerror("vectorfind([1 2], string([1 2]))", msg);
