// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16586 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16586
//
// <-- Short Description -->
// mfile2sci: The prettyprintoutput flag badly managed appended comments

code = ["% Comment #1"
        "a = 2; b = 1;      % this is a test"
        ];
mfile = TMPDIR + "/bug_16586";
mputl(code, mfile+".m");

mfile2sci(mfile+".m", TMPDIR,,,,%t);    // prettyfied
res = mgetl(mfile+".sci")
ref = [""
       "// Comment #1"
       "a = 2;"
       "b = 1;  // this is a test"];
assert_checkequal(res, ref);

mfile2sci(mfile+".m", TMPDIR,,,,%f);    // not prettyfied
res = mgetl(mfile+".sci");
ref = [""
       "// Comment #1"
       "a = 2;b = 1;  // this is a test"];
assert_checkequal(res, ref);
