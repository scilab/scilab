// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14743 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14743
//
// <-- Short Description -->
// test_run(..,"show_error") did not give details for "failed: Slave Scilab exited.." errors

// test_run("development_tools", "bug_14743_b", "show_error");
// This does not work, because the test report includes some test duration
// that differs from one run to the other. Hence, .dia is always different from
// .dia.ref. BTW, the "short_summary" option cancels the "show_error" one.
// This test can unlikely be automatized ==> Set Interactive

tdir = TMPDIR+"/test_bug_14743";
mkdir(tdir);
mkdir(tdir+"/tests/nonreg_tests");
tmp = [
"// <-- Non-regression test for bug 14743_b -->"
"//"
"// <-- Bugzilla URL -->"
"// http://bugzilla.scilab.org/14743"
"//"
"// <-- Short Description -->"
"// test_run(,""show_error"") did not give details for ""failed: Slave Scilab exited"" errors"
"//"
"// <-- CLI SHELL MODE -->"
"// <-- ENGLISH IMPOSED -->"
""
"a = b/c   // This must yield an error, b and c not being defined"
""
];
tfile = tdir+"/tests/nonreg_tests/bug_14743_b.tst";
xmlfile = tdir+"/bug_14743_b.xml";
mputl(tmp, tfile);
test_run(tdir, "bug_14743_b", "show_error", xmlfile);

if grep(mgetl(xmlfile), "Undefined variable") == [] then pause, end
