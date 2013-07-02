// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): 1 to 3 expected."), "h5readattr");
assert_checkerror("h5readattr()",msgerr,77);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected."), "h5readattr");
assert_checkerror("h5readattr(42)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid number of argument(s): 3 expected."), "h5readattr");
assert_checkerror("h5readattr(""42"")",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #2: A string expected."), "h5readattr");
assert_checkerror("h5readattr(""42"",42,42)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #3: A string expected."), "h5readattr");
assert_checkerror("h5readattr(""42"",""42"",42)",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s.\nHDF5 description: unable to find a valid file signature."), "h5readattr","42");
assert_checkerror("h5readattr(""42"",""42"",""42"")",msgerr,999);

x = int8(matrix(1:80, 10, 8));
save(TMPDIR + "/x.sod", "x");
version = string(getversion('scilab'));
version = getversion()+" "+strcat([version(1),version(2),version(3)],'.');

a = h5open(TMPDIR + "/x.sod");

scilab_version = h5readattr(a, "/", "SCILAB_scilab_version");
assert_checkequal(scilab_version,version);

scilab_class = h5readattr(a.root.x, "SCILAB_Class");
assert_checkequal(scilab_class,"integer");

msgerr = msprintf(gettext("%s: Cannot open attribute: %s\n"), ..
                "h5readattr","test");
assert_checkerror("h5readattr(a.root.x, ""test"")",msgerr,999);

msgerr = msprintf(gettext("%s: Invalid name: %s"), "h5readattr",".");
assert_checkerror("h5readattr(a.root.x)",msgerr,999);

msgerr = msprintf(gettext("%s: Cannot open attribute: %s"), ..
            "h5readattr","Attributes");
assert_checkerror("h5readattr(a.root.x,""Attributes"")",msgerr,999);
//After closing, all h5readattr will fail
h5close(a);

msgerr = msprintf(gettext("%s: Invalid H5Object.\n"), "h5readattr");
assert_checkerror("h5readattr(a, ""/"", ""SCILAB_scilab_version"")",msgerr,999);
msgerr = msprintf(gettext("%s: Invalid H5Object.\n"), "%H5Object_e");
assert_checkerror("h5readattr(a.root.x, ""SCILAB_Class"")",msgerr,999);
assert_checkerror("h5readattr(a.root.x, ""test"")",msgerr,999);
assert_checkerror("h5readattr(a.root.x)",msgerr,999);
assert_checkerror("h5readattr(a.root.x,""Attributes"")",msgerr,999);
msgerr = msprintf(gettext("Undefined variable: %s\n"), "b");
assert_checkerror("h5readattr(b)",msgerr,4);

//on a non hdf5 variable
b=1;
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: A string expected.\n"), "h5readattr");
assert_checkerror("h5readattr(b)",msgerr,999);
msgerr = msprintf(gettext("%s: Wrong type for input argument #1: " ..
                    +"A string expected.\n"), "h5readattr");
assert_checkerror("h5readattr(b,""/"")",msgerr,999);
b="hello";
msgerr = msprintf(gettext("%s: Invalid hdf5 file: %s.\n"), "h5readattr",b);
assert_checkerror("h5readattr(b,""/"")",msgerr,999);


assert_checkequal(deletefile(TMPDIR+"/x.sod"),%T);
