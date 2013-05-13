// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 2 to 3 expected."), "h5exists");
assert_checkerror("h5exists()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 2 to 3 expected."), "h5exists");
assert_checkerror("h5exists(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5exists");
assert_checkerror("h5exists(42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5exists");
assert_checkerror("h5exists(""42"",42)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s.\nHDF5 description: unable to find a valid file signature."),"h5exists","42");
assert_checkerror("h5exists(""42"",""42"")",msgerr,999);

x = 123;
y = 321;
save(TMPDIR + "/xy.sod", "x", "y");
a = h5open(TMPDIR + "/xy.sod", "r");

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 2 to 3 expected."), "h5exists");
assert_checkerror("h5exists(a)",msgerr,77);
assert_checkfalse(h5exists(a,"42"));

b= [%T %T %F];
assert_checkequal(h5exists(a, ["x" "y" "z"]),b);
assert_checkfalse(h5exists(a, "/tmp"));
assert_checktrue(h5exists(a, "/x"));
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5exists");
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
