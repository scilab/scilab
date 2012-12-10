// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11976
//
// <-- Short Description -->
// decimal parameter must be '.' or ','
//

some_data = ["67200.99976"];
iErr = execstr("data_time = csvTextScan(some_data, ""."", """", ""double"")", "errcatch");
assert_checktrue(iErr <> 0);
data_time = csvTextScan(some_data, ".", ",", "double");
