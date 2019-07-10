// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16144 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16144
//
// <-- Short Description -->
// Addition of sparse matrices gives incorrect results

message = msprintf(_("Inconsistent row/column dimensions.\n"));
assert_checkerror("sparse([3,4],9) + sparse([1,2],5)",message);
assert_checkequal(sparse([3,4],9,[4 4]) + sparse([1,2],5,[4,4]), sparse([3,4;1,2],[9,5],[4 4]))

