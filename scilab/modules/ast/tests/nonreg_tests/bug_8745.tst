// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//
// <-- Non-regression test for bug 8745 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8745
//
// <-- Short Description -->
//
// Extracting from an empty matrix automatically returns an empty matrix

// <-- CLI SHELL MODE -->

A=[];
errmsg=msprintf(_("Invalid index.\n"));
assert_checkerror("A(-1)", errmsg);
assert_checkerror("A(cos)", errmsg);
assert_checkerror("A(""x"")", errmsg);
assert_checkequal(A(1:$), []);
assert_checkequal(A($), []);
assert_checkequal(A(%t), []);

assert_checkerror("A(-1, -1)", errmsg);
assert_checkerror("A(1, -1)", errmsg);
assert_checkerror("A(-1, 1)", errmsg);
assert_checkerror("A(cos, cos)", errmsg);
assert_checkerror("A(cos, 1)", errmsg);
assert_checkerror("A(1, cos)", errmsg);
assert_checkerror("A(""x"", ""x"")", errmsg);
assert_checkerror("A(1, ""x"")", errmsg);
assert_checkerror("A(""x"", 1)", errmsg);
assert_checkequal(A(1:$,1:$), []);
assert_checkequal(A(1:$,1), []);
assert_checkequal(A(1, 1:$), []);
assert_checkequal(A($, $), []);
assert_checkequal(A(1, $), []);
assert_checkequal(A($, 1), []);
assert_checkequal(A(%t, %t), []);
assert_checkequal(A(1, %t), []);
assert_checkequal(A(%t, 1), []);
