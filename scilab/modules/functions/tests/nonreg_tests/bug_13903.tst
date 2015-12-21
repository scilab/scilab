// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13903 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13903
//
// <-- CLI SHELL MODE -->
//
// <-- Short Description -->
// get_function_path returns a wrong path because of a missing file separator

path=get_function_path("get_function_path");
assert_checktrue(isfile(path));
