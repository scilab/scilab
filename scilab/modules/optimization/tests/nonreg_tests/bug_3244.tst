// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3244 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3244
//
// <-- Short Description -->
//    Bad outputs for the qpsolve/qp_solve functions
// =============================================================================

assert_checkfalse(execstr("qpsolve", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve", refMsg);

assert_checkfalse(execstr("qpsolve(1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1)", refMsg);

assert_checkfalse(execstr("qpsolve(1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1, 1)", refMsg);

assert_checkfalse(execstr("qpsolve(1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1, 1, 1)", refMsg);

assert_checkfalse(execstr("qpsolve(1, 1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1, 1, 1, 1)", refMsg);

assert_checkfalse(execstr("qpsolve(1, 1, 1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1, 1, 1, 1, 1)", refMsg);

assert_checkfalse(execstr("qpsolve(1, 1, 1, 1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qpsolve", 7);
assert_checkerror("qpsolve(1, 1, 1, 1, 1, 1)", refMsg);


assert_checkfalse(execstr("qp_solve", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qp_solve", 5);
assert_checkerror("qp_solve", refMsg);

assert_checkfalse(execstr("qp_solve(1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qp_solve", 5);
assert_checkerror("qp_solve(1)", refMsg);

assert_checkfalse(execstr("qp_solve(1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qp_solve", 5);
assert_checkerror("qp_solve(1, 1)", refMsg);

assert_checkfalse(execstr("qp_solve(1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qp_solve", 5);
assert_checkerror("qp_solve(1, 1, 1)", refMsg);

assert_checkfalse(execstr("qp_solve(1, 1, 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "qp_solve", 5);
assert_checkerror("qp_solve(1, 1, 1, 1)", refMsg);

assert_checkfalse(execstr("qp_solve(1, 1, [], 1, 1)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: %d column(s) expected for matrix %s.\n"), "qp_solve", 3, 1, "C");
assert_checkerror("qp_solve(1, 1, [], 1, 1)", refMsg);

assert_checkfalse(execstr("qp_solve(1, 2, 3, 4, 5)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s must be an integer in the range 0 to %d.\n"), "qp_solve", 5, "me", 1);
assert_checkerror("qp_solve(1, 2, 3, 4, 5)", refMsg);

