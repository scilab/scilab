// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 15998 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15998
//
// <-- Short Description -->
// mfile2sci() and translatepaths() yielded an inconvenient warning when
// converting atan()

File = TMPDIR + "/bug_15998_atan";
mputl(["x = 4;" "y = atan(x);"], File + ".m");
mfile2sci(File + ".m", TMPDIR)
t = mgetl(File + ".sci");
tref = ["x = 4;"
        "// !! L.2: If x is imaginary and its module is > 1"
        "// !!       the real part of atan(x) is -pi/2 in Scilab instead of +pi/2 in Matlab."
        "y = atan(x);"
        ];
assert_checkequal(t($-3:$), tref);
deletefile(File + [".m" ".sci"]);
