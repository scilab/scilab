// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 4 expected."), "h5label");
assert_checkerror("h5label()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 4 expected."), "h5label");
assert_checkerror("h5label(42)",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string or a H5Object expected."), "h5label");
assert_checkerror("h5label(42,42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5label");
assert_checkerror("h5label(""42"",42,42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #4: a row of strings expected."), "h5label");
assert_checkerror("h5label(""42"",""42"",42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s.\nHDF5 description: unable to find a valid file signature."), "h5label","42");
assert_checkerror("h5label(""42"",""42"",42,[""42""])",msgerr,999);

a = h5open(TMPDIR + "/test.h5", "w");
msgerr = msprintf(gettext("%s: Invalid name: %s."), "h5label","42");
assert_checkerror("h5label(a,""42"",42,[""42""])",msgerr,999);

x = uint8(1:24);
h5dataset(a, "Dset_1", [1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1; 1 2 3 4], x, [1 2 3 4 ; 1 2 3 4 ; 1 1 1 1 ; 1 1 1 1 ; 1 1 1 1 ; 1 2 3 4]);
h5label(a, "Dset_1", [1 3], ["Hello" "World"]);
assert_checkequal(a.root.Dset_1.DIMENSION_LABELS.data,["Hello" "" "World" ""]);
h5close(a)
