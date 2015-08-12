// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13780 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13780
//
// <-- Short Description -->
//    size with two input and output arguments did not return an error.
// =============================================================================

assert_checkfalse(execstr("[r,c] = size(ones(4,2), ""*"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "size", 1);
assert_checkerror("[r,c] = size(ones(4,2), ""*"")", refMsg);

assert_checkfalse(execstr("[r,c] = size(ones(4,2), ""r"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "size", 1);
assert_checkerror("[r,c] = size(ones(4,2), ""*"")", refMsg);

assert_checkfalse(execstr("[r,c] = size(ones(4,2), ""c"")", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "size", 1);
assert_checkerror("[r,c] = size(ones(4,2), ""*"")", refMsg);
