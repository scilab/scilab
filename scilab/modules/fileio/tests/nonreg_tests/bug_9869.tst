// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9869 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9869
//
// <-- Short Description -->
// fscanf did not check number of input arguments

wMode = warning("query");
warning("off");

msgError = gettext("%s: Wrong number of input arguments: %d expected.\n");
assert_checkerror ("fscanf()", msgError , [] , "fscanf" , 2);

msgError = gettext("%s: Wrong number of input arguments: %d expected.\n");
assert_checkerror ("fscanf(TMPDIR + ""/bug_9869.dat"")", msgError , [] , "fscanf" , 2);

mputl(string(1:4), TMPDIR + "/bug_9869.dat");
R = fscanf(TMPDIR + "/bug_9869.dat","%d");
assert_checkequal(R, 1);

warning(wMode);

