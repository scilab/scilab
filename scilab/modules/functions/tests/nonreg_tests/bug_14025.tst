// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14025 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14025
//
// <-- Short Description -->
// head_comments did not take into account compiled functions
// <-- CLI SHELL MODE -->
function test()
    // heading comment L#1
    // heading comment L#2
    disp("test function");
endfunction

assert_checktrue(execstr("head_comments(""test"");", "errcatch") == 0);
