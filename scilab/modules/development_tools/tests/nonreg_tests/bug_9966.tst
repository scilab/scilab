// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9635 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9635
//
// <-- Short Description -->
// The default value of comptype in assert_checkalmostequal is wrongly chosen.

A = [1e10 1e-2];
B = [1e10 2e-2];
instr = "assert_checkalmostequal(A,B)";
lclmsg = "%s: Assertion failed: expected = %s while computed = %s";
strA = "[" + string(A(1)) + " ...]";
strB = "[" + string(B(1)) + " ...]";
assert_checkerror(instr,lclmsg,[],"assert_checkalmostequal",strA,strB);
