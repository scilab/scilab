// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 1 to 6 expected."), "h5read");
assert_checkerror("h5read()",msgerr,77);
msgerr = msprintf(gettext("%s: Invalid number of input argument."), "h5read");
assert_checkerror("h5read(42)",msgerr,999);
assert_checkerror("h5read(""42"")",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5read");
assert_checkerror("h5read(42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Second argument must be a dataset name."), "h5read");
assert_checkerror("h5read(""42"",42)",msgerr,999);

x = int8(matrix(1:80, 10, 8));
save(TMPDIR + "/x.sod", "x"); // SOD files are HDF5 ones 	
y = [1 2;3 4];
save(TMPDIR + "/y.sod", "y");

a = h5open(TMPDIR + "/x.sod");
msgerr = msprintf(gettext("%s: Invalid object: not a dataset."), "h5read");
assert_checkerror("h5read(a)",msgerr,999);
assert_checkequal(h5read(a, "/x"),x');
assert_checkequal(h5read(a, "/x", [3 4], [5 3]),x(4:6,3:7)');
msgerr = msprintf(gettext("%s: Invalid selection."), "h5read");
assert_checkerror("h5read(a,""/x"", [7 9], [10 12])",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid name: %s."), "h5read", "/y");
assert_checkerror("h5read(a,""/y"")",msgerr,999);
assert_checkequal(h5read(a, "/x",[2 3],[3 3],[3 2]),int8([13 15 17;43 45 47;73 75 77]));
msgerr = msprintf(gettext("%s: Invalid selection."), "h5read");
assert_checkerror("h5read(a,""/x"",[2 3],[3 3],[3 5])",msgerr,999);
h5close(a);

msgerr = msprintf(gettext("%s: Can not print H5Object: invalid object."), "h5read");
assert_checkerror("h5read(a)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid number of input argument."), "h5read");
assert_checkerror("h5read(TMPDIR+""/y.sod"")",msgerr,999);
assert_checkequal(h5read(TMPDIR+"/y.sod","/y"),[1,2;3,4]');
