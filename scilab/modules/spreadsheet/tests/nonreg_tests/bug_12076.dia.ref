//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// 
// <-- JAVA NOT MANDATORY -->
//
// <-- Non-regression test for bug 12076 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12076
//
// <-- Short Description -->
// csvRead does not ignore blank lines
//
filename=SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_12076.csv";
a=csvRead(filename,";");
assert_checkequal(size(a),[2, 4]);
ref=[%nan,%nan,%nan,%nan;1,2,3,%nan];
assert_checkequal(a,ref);
