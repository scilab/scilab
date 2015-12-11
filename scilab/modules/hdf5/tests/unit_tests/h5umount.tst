// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5umount", 2);
assert_checkerror("h5umount()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5umount", 2);
assert_checkerror("h5umount(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A H5Object expected.\n"), "h5umount", 1);
assert_checkerror("h5umount(42,42)",msgerr,999);
a = h5open(TMPDIR + "/test.h5", "w");
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5umount", 2);
assert_checkerror("h5umount(a,42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5umount", msprintf(gettext("Cannot unmount the file at location: %s"), "42"));
msgerr($+1) = gettext("HDF5 description") + ": " + "object ''42'' doesn''t exist.";
assert_checkerror("h5umount(a,""42"")",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A H5Object expected.\n"), "h5umount", 1);
assert_checkerror("h5umount(""42"",""42"")",msgerr,999);


h5group(a, "/mnt");
b = h5open(TMPDIR + "/test1.h5", "w");
h5group(b, "Group_1");
h5write(b, "Group_1/Dataset_1", [1 2;3 4]);

h5mount(a, "/mnt", b);
assert_checkequal(a.root.mnt.Group_1.Dataset_1.data,[1 2;3 4]);
h5write(a.root.mnt.Group_1, "Dataset_1", [10 11;12 13]);
assert_checkequal(a.root.mnt.Group_1.Dataset_1.data,[10 11;12 13]);
assert_checkequal(b.root.mnt.Group_1.Dataset_1.data,[10 11;12 13]);
h5umount(a, "/mnt");
assert_checkequal(a.root.mnt.Groups,[]);
assert_checkequal(b.root.Group_1.Dataset_1.data,[10 11;12 13]);
h5rm(a, "/mnt");
msgerr = msprintf(gettext("%s: Error in retrieving field content:\n%s\n"), "%H5Object_e", msprintf(gettext("Invalid field: %s"), "mnt"));
assert_checkerror("a.root.mnt",msgerr,999);
assert_checkequal(b.root.Group_1.Dataset_1.data,[10 11;12 13]);

h5group(a,"/mnt");
h5mount(a,"/mnt",b);


h5close(a, b);
assert_checkequal(h5ls(TMPDIR+"/test.h5","/"),["mnt" "group"]);
assert_checkequal(h5ls(TMPDIR+"/test.h5","/mnt"),[]);
assert_checkequal(h5ls(TMPDIR+"/test1.h5","/"),["Group_1" "group"]);

a = h5open(TMPDIR+"/test.h5","a");
for i=1:10
    execstr("ref_"+string(i)+"=h5open("""+TMPDIR+"/test_"+string(i)+".h5"",""w"")");
    execstr("h5group(ref_"+string(i)+",""/Group_"+string(i)+""")");
    execstr("h5mount(a,""/mnt"",ref_"+string(i)+")");
    assert_checkequal(a.root.mnt.Groups,"Group_"+string(i));
end
for i=10:-1:1
    execstr("h5umount(a,""/mnt"")");
    if i ~= 1 then
        assert_checkequal(a.root.mnt.Groups,"Group_"+string(i-1));
    end
    execstr("h5close(ref_"+string(i)+")");
end
