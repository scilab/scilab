// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5149 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5149
//
// <-- Short Description -->
// Calls to exec functions are no more logged by diary

test_run("m2sci","bug_683","create_ref");

// Just check if the exec calls are logged in
// SCI/modules/m2sci/tests/nonreg_tests/bug_683.dia.ref
