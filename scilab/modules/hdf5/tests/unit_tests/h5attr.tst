// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 4 to 5 expected."), "h5attr");
assert_checkerror("h5attr()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 4 to 5 expected."), "h5attr");
assert_checkerror("h5attr(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string or a H5Object expected."), "h5attr");
assert_checkerror("h5attr(42,42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5attr");
assert_checkerror("h5attr(""42"",42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5attr");
assert_checkerror("h5attr(""42"",""42"",42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s."), "h5attr","42");
assert_checkerror("h5attr(""42"",""42"",""42"",42)",msgerr,999);

x = 1:5 + %pi;
save(TMPDIR + "/x.sod", "x");

a = h5open(TMPDIR + "/x.sod");

msgerr = msprintf(gettext("%s: Invalid name: %s."), "h5attr","42");
assert_checkerror("h5attr(a,""42"",""42"",42)",msgerr,999);


h5attr(a, ".", "My Attribute", int8(123));
assert_checkequal(a.root("My Attribute").Data,int8(123));
msgerr = msprintf(gettext("%s: Attribute %s already exists."), "h5attr", "My Attribute");
assert_checkerror("h5attr(a, ""."", ""My Attribute"", %pi)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5attr");
assert_checkerror("h5attr(a,""."",a.root.Attributes,int8(2))",msgerr,999);

h5group(a,"Group_1");
h5attr(a,"/Group_1","New",[5 6]);
assert_checkequal(a.root.Group_1.New.Data,[5 6]');
h5write(a,"Group_1/Dset_1",[1 2;3 4]);
h5attr(a,"Group_1/Dset_1","Second",[7 8]);
assert_checkequal(a.root.Group_1.Dset_1.Second.Data,[7 8]');

msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s."), "h5attr",TMPDIR+"/y.sod");
assert_checkerror("h5attr(TMPDIR+""/y.sod"",""."",""New"",int8(16))",msgerr,999);
h5attr(TMPDIR+"/x.sod", ".", "New", int8(8));
assert_checkequal(a.root.New.Data,int8(8));


h5attr(a, "/x", "My Dataset Attribute", int16(321));
h5close(a);

// h5writeattr testing
x = 1:5 + %pi;
save(TMPDIR + "/x.sod", "x");
a = h5open(TMPDIR + "/x.sod");
h5writeattr(a, ".", "My Attribute", int8(123));
assert_checkequal(a.root("My Attribute").Data,int8(123));
msgerr = msprintf(gettext("%s: Attribute %s already exists."), "h5writeattr", "My Attribute");
assert_checkerror("h5writeattr(a, ""."", ""My Attribute"", %pi)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5writeattr");
assert_checkerror("h5writeattr(a,""."",a.root.Attributes,int8(2))",msgerr,999);

h5group(a,"Group_1");
h5writeattr(a,"/Group_1","New",[5 6]);
assert_checkequal(a.root.Group_1.New.Data,[5 6]');
h5write(a,"Group_1/Dset_1",[1 2;3 4]);
h5writeattr(a,"Group_1/Dset_1","Second",[7 8]);
assert_checkequal(a.root.Group_1.Dset_1.Second.Data,[7 8]');

msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s."), "h5writeattr",TMPDIR+"/y.sod");
assert_checkerror("h5writeattr(TMPDIR+""/y.sod"",""."",""New"",int8(16))",msgerr,999);
h5attr(TMPDIR+"/x.sod", ".", "New", int8(8));
assert_checkequal(a.root.New.Data,int8(8));

h5writeattr(a, "/x", "My Dataset Attribute", int16(321));
h5close(a);
