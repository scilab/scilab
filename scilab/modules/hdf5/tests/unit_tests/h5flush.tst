// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "h5flush", 1, 2);
assert_checkerror("h5flush()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #%d: A HDF5 object expected.\n"), "h5flush", 1);
assert_checkerror("h5flush(42)",msgerr,999);

deletefile(TMPDIR + "/test.h5");
deletefile(TMPDIR + "/test1.h5");

a = h5open(TMPDIR + "/test.h5", "w");
x = uint8(1:24);
h5dataset(a, "Dset1", [1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1; 1 2 3 4], x, [1 2 3 4 ; 1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1 ; 1 2 3 4]);
assert_checkequal(h5ls(a,".","d"),["Dset1"]);

b = h5open(TMPDIR + "/test1.h5");
h5group(a, "Grp_1");
h5write(a.root.Grp_1, "Dset_1", [1 2 ; 3 4]);
h5flush(a);
h5group(b, "BGrp");
h5write(b("/BGrp"), "BDset", 11:18);
h5close(b)

h5mv(a.root.Grp_1, "Dset_1", a, "Dset_2")
assert_checkequal(a.root.Datasets,["Dset1";"Dset_2"]);
assert_checkequal(a.root.Dset_2.data,[1 2;3 4]);
h5write(a.root.Grp_1, "Dset_1", [1 2 ; 3 4]);
h5flush(a);
h5mv(a.root.Grp_1, "Dset_1", a, "Dset_3")
assert_checkequal(a.root.Dset_3.data,[1 2;3 4]);
h5close(a);
