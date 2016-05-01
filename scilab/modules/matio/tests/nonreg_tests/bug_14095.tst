// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 14095 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14095
//
// <-- Short Description -->
// Load a fig file with loadmatfile makes Scilab crash
fig_file = fullfile(SCI,"modules","matio", "tests", "nonreg_tests", "bug_14095.fig");
assert_checktrue(execstr("loadmatfile(""-mat"", fig_file)", "errcatch") == 0); 
