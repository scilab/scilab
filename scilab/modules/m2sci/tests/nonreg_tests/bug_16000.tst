// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 16000 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16000
//
// <-- Short Description -->
// mfile2sci() and translatepaths() converted acoth() using atanh() and yielding
// a wrong warning message.

File = TMPDIR + "/bug_16000_acoth";
mputl(["x = 4;" "y = acoth(x);"], File + ".m");
mfile2sci(File + ".m", TMPDIR)
t = mgetl(File + ".sci");
ref = ["// !! L.2: If x is in [0,1)"
       "// !!   imag(acoth(x)) is -pi/2 in Scilab instead of +pi/2 in Matlab."
       "y = acoth(x);"
       ];
assert_checkequal(t($-2:$), ref);
deletefile(File+[".m" ".sci"]);
