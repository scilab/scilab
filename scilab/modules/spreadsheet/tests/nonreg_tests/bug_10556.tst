// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10556 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10556
//
// <-- Short Description -->
// readxls produced wrong results
//

spreadsheet = readxls(SCI + "/modules/spreadsheet/tests/nonreg_tests/bug_10556.xls");
assert_checkequal(size(spreadsheet(1)), [12 10]);
xls = spreadsheet(1);
assert_checkequal(xls(2, 2), 3.06);
assert_checkequal(xls(2,1), "OK++ AL 1067");