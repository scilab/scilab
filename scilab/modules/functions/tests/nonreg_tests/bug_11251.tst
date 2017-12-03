// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 11251 -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11251
//
// <-- Short Description -->
// execstr() could kill the stack

A = [];
errmsg = "Incompatible assignation: trying to assign 1 values in 2 variables.";
assert_checkerror("execstr(""[A($+1),B] = 1;"")", errmsg);
"[A($+1),B] = 1;"
assert_checkerror("[A($+1),B] = 1;", errmsg);
clear
