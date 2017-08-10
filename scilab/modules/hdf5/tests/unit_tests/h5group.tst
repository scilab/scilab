// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5group", 2);
assert_checkerror("h5group()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5group", 2);
assert_checkerror("h5group(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5group", 1);
assert_checkerror("h5group(42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "h5group", 2);
assert_checkerror("h5group(""42"",42)",msgerr,999);
msgerr = msprintf(gettext("%s: %s\n"), "h5group", msprintf(gettext("Invalid hdf5 file: %s."), "42"));
assert_checkerror("h5group(""42"",""42"")",msgerr,999);

a = h5open(TMPDIR + "/test_group.h5", "a");
gnames = "Group_" + string(1:10);
sgnames = "SubGroup_" + string(1:10);
names = gnames;
for s=gnames
    names = [names s + "/" + sgnames];
end
h5group(a, names);
groups = cat(2,["Group_1" "Group_10"], "Group_"+string(2:9));
assert_checkequal(a.root.Groups,groups');
subgroups = cat(2,["SubGroup_1" "SubGroup_10"], "SubGroup_"+string(2:9));
for i=groups
    execstr("assert_checkequal(a.root."+i+".Groups,subgroups'')");
end

h5close(a);

gnames2 = "Group_" + string(11:15);
sgnames2 = "SubGroup_" + string(11:15);
names2 = gnames2;
for s=gnames2
    names2 = [names2 s + "/" + sgnames2];
end
h5group(TMPDIR+"/test_group.h5",names2);
a = h5open(TMPDIR + "/test_group.h5", "a");
groups = cat(2,["Group_1"],"Group_"+string(10:15), "Group_"+string(2:9));
partgroups = "Group_"+string(11:15);
assert_checkequal(a.root.Groups,groups');
subgroups = "SubGroup_"+string(11:15);
for i=partgroups
    execstr("assert_checkequal(a.root."+i+".Groups,subgroups'')");
end
h5close(a);
