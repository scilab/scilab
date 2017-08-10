//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon Marchetto
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// English imposed to compare error message
// <-- ENGLISH IMPOSED -->
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12246 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12246
//
// <-- Short Description -->
// Check that an error message is returned when
//reading invalid excel files (which have declared wrong colum/row count)

xlsPath = SCI + "/modules/spreadsheet/tests/nonreg_tests/bug_12246.xls";
[fd,SST,Sheetnames,Sheetpos] = xls_open(xlsPath);

errMsg = "xls_read: Failed to read expected data, may be invalid xls file.";
assert_checkerror("[Value,TextInd] = xls_read(fd,Sheetpos(1))", errMsg);
