// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 11999 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11999
//
// <-- Short Description -->
// csvRead does not ignore blank lines
//
filename = SCI + "/modules/spreadsheet/tests/nonreg_tests/bug_11999.csv";

csvDefault("blank", "off"); // Default behavior in 5.4.0, changed 5.4.1 to "on"
errMsg = msprintf(gettext("%s: can not read file %s: Error in the column structure\n"), "csvRead", filename);
assert_checkerror("a=csvRead(filename, "";"", [], ""string"");", errMsg, 999);

csvDefault("blank", "on");
a=csvRead(filename, ";", [], "string");
assert_checkequal(a,["foo", "bar", "foo"; "foo2", "bar2", "bar3"]);
