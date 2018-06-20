// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 15484 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/15484
//
// <-- Short Description -->
// isdir("w:"), isdir("w:\"), and  isdir("w:/") returned %T whenever the w:
// drive does not exist.

// This assumes that the w:/ drive actually does not exist..

assert_checkfalse(isdir("w:"));
assert_checkfalse(isdir("w:\"));
assert_checkfalse(isdir("w:/"));
assert_checkfalse(isdir("w:\."));
assert_checkfalse(isdir("w:/."));

assert_checktrue(isdir("c:"));
assert_checktrue(isdir("c:/"));
assert_checktrue(isdir("c:\"));
