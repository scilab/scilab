// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9020 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9020
//
// <-- Short Description -->
// "exists" does not accept matrix input as first argument

A=1; B=2; C=3;

assert_checkequal(exists(["A", "B", "C", "D"]), [1, 1, 1, 0]);
assert_checkequal(exists(["A"; "B"; "C"; "D"]), [1; 1; 1; 0]);
assert_checkequal(exists(["D", "A", "B", "C"]), [0, 1, 1, 1]);
assert_checkequal(exists(["D", "A"; "B", "C"]), [0, 1; 1, 1]);

assert_checkequal(isdef(["A", "B", "C", "D"]), [%t, %t, %t, %f]);
assert_checkequal(isdef(["A"; "B"; "C"; "D"]), [%t; %t; %t; %f]);
assert_checkequal(isdef(["D", "A", "B", "C"]), [%f, %t, %t, %t]);
assert_checkequal(isdef(["D", "A"; "B", "C"]), [%f, %t; %t, %t]);
