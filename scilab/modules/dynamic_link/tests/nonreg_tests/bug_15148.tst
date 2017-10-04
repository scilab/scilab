// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15148 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15148
//
// <-- Short Description -->
// link() doesn't return any error for a wrong flag argument value

instr = "link(""foodll"", ""foo"", ""dummy"")";
expected_errmsg = 'link: Wrong value for input argument #3: ''f'' or ''c'' expected.';
assert_checkerror(instr, expected_errmsg);
