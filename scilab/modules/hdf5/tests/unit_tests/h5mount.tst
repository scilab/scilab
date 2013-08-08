// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5mount", 3);
assert_checkerror("h5mount()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5mount", 3);
assert_checkerror("h5mount(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A H5Object expected.\n"), "h5mount", 1);
assert_checkerror("h5mount(42,42,42)",msgerr,999);

a = h5open(TMPDIR + "/test.h5", "w");

msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A H5Object expected.\n"), "h5mount", 3);
assert_checkerror("h5mount(a,""42"",42)",msgerr,999);


h5group(a, "/mnt");
b = h5open(TMPDIR + "/test1.h5", "w");
h5group(b, "Group_1");
h5write(b, "Group_1/Dataset_1", uint32([1 2;3 4]));
c = h5open(TMPDIR + "/test2.h5", "w");
h5group(c, "Group_2");
h5write(c, "Group_2/Dataset_2", uint32([14 15 16;17 18 19]));

h5mount(a, "/mnt", b);
assert_checkequal(a.root.mnt.Groups,"Group_1");
msgerr = msprintf(gettext("%s: %s\n"), "h5mount", msprintf(gettext("Cannot mount the file: %s"), TMPDIR + filesep() + "test1.h5"));
msgerr($+1) = gettext("HDF5 description") + ": " + "file is already mounted.";
assert_checkerror("h5mount(a, ""/mnt"", b)",msgerr,999);
h5mount(a, "/mnt", c);
assert_checkequal(a.root.mnt.Groups,"Group_2");
msgerr = msprintf(gettext("%s: %s\n"), "h5mount", msprintf(gettext("Invalid location: %s"),"/test"));
assert_checkerror("h5mount(a,""/test"",c)",msgerr,999);
msgerr = msprintf(gettext("%s: Error in retrieving field content:\n%s\n"), "%H5Object_e", msprintf(gettext("Invalid field: %s"), "Group_1"));
assert_checkerror("h5write(a.root.mnt.Group_1, ""Dataset_1"", uint32([10 11;12 13]))",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5write", gettext("Cannot write data in the dataset."));
msgerr($+1) = gettext("HDF5 description") + ": " + "src and dest data spaces have different sizes.";
assert_checkerror("h5write(a.root.mnt.Group_2, ""Dataset_2"", uint32([10 11;12 13]))",msgerr,999);
h5write(a.root.mnt.Group_2, "Dataset_2", uint32([10 11 2;12 13 9]));
h5umount(a, "/mnt");
assert_checkequal(a.root.mnt.Groups,"Group_1");
assert_checkequal(c.root.Group_2.Dataset_2.data,uint32([10 11 2;12 13 9]));
h5rm(a, "/mnt")
assert_checkequal(c.root.Group_2.Dataset_2.data,uint32([10 11 2;12 13 9]));
h5close(a, b, c)
assert_checkequal(h5read(TMPDIR+"/test2.h5","/Group_2/Dataset_2"),uint32([10 11 2;12 13 9]));

