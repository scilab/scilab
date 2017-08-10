// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5exists", 2, 3);
assert_checkerror("h5exists()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5exists", 2, 3);
assert_checkerror("h5exists(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5exists", 1);
assert_checkerror("h5exists(42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5exists", 2);
assert_checkerror("h5exists(""42"",42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5exists", msprintf(gettext("Invalid hdf5 file: %s."), "42"));
assert_checkerror("h5exists(""42"",""42"")",msgerr,999);

x = 123;
y = 321;
save(TMPDIR + "/xy.sod", "x", "y");
a = h5open(TMPDIR + "/xy.sod", "r");

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5exists", 2, 3);
assert_checkerror("h5exists(a)",msgerr,77);
assert_checkfalse(h5exists(a,"42"));

b= [%T %T %F];
assert_checkequal(h5exists(a, ["x" "y" "z"]),b);
assert_checkfalse(h5exists(a, "/tmp"));
assert_checktrue(h5exists(a, "/x"));
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5exists", 2);
assert_checkerror("h5exists(a,a.root.x)",msgerr,999);
c = [%T %T];
assert_checkequal(h5exists(a, "/", ["SCILAB_sod_version", "SCILAB_scilab_version"]),c);
assert_checktrue(h5exists(a, "/x", "SCILAB_Class"));
h5close(a);

assert_checkequal(h5exists(TMPDIR+"/xy.sod", ["x" "y" "z"]),b);
assert_checkfalse(h5exists(TMPDIR+"/xy.sod", "/tmp"));
assert_checktrue(h5exists(TMPDIR+"/xy.sod", "/x"));
c = [%T %T];
assert_checkequal(h5exists(TMPDIR+"/xy.sod", "/", ["SCILAB_sod_version", "SCILAB_scilab_version"]),c);
assert_checktrue(h5exists(TMPDIR+"/xy.sod", "/x", "SCILAB_Class"));
