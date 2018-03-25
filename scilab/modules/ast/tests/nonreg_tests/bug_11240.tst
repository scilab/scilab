// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 11240 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11240
//
// <-- Short Description -->
// A = []; clear B;  then [A($+1),B] = 1; crashed Scilab

A = [];
clear B
assert_checktrue(execstr("[A($+1),B] = 1;", "errcatch")~=0);
