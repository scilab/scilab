// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11324 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11324
//
// <-- Short Description -->
// restore old file ( bon format ) could fail due to new mechanism in %_sodload

load("SCI/modules/io/tests/nonreg_tests/bug_11324.mat");
assert_checkequal(filename, "oh_no_i_found_a_bug");