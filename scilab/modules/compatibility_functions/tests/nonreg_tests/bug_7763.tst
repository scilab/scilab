// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7763 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7763
//
// <-- Short Description -->
//    mtlb_max and mtlb_min cannot handle %i*0

r = mtlb_max(1 + %i * 0, 0);
assert_checkequal(r, complex(1));

r = mtlb_min(1 + %i * 0, 0);
assert_checkequal(r, 0);

assert_checkfalse(execstr("[r,k] = mtlb_max(1 + %i * 0, 0)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "mtlb_max", 1); 
assert_checkerror("[r,k] = mtlb_max(1 + %i * 0, 0)", refMsg);

assert_checkfalse(execstr("[r,k] = mtlb_min(1 + %i * 0, 0)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "mtlb_min", 1); 
assert_checkerror("[r,k] = mtlb_min(1 + %i * 0, 0)", refMsg);






