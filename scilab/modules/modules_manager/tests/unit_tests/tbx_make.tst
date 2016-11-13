// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK ERROR OUTPUT -->
// <-- ENGLISH IMPOSED -->

// Test the compilation of a simple toolbox with tbx_make()

// Copying the foobox from the test environment
mytb_dir = TMPDIR + "/foobox";
if isdir(mytb_dir)
    removedir(mytb_dir);
end
status = copyfile(SCI + "/modules/modules_manager/tests/unit_tests/foobox/", mytb_dir);
assert_checkequal(status, 1);
tbx_make(mytb_dir);

exec(mytb_dir + "/loader.sce");
b = foo("testing it works", 42);
baz("World");

assert_checktrue(b);

// testing default compilation
mytb_dir = TMPDIR + "/testbox";
if isdir(mytb_dir)
    removedir(mytb_dir);
end
status = copyfile(SCI + "/modules/modules_manager/tests/unit_tests/testbox/", mytb_dir);
assert_checkequal(status, 1);
tbx_make(mytb_dir);

exec(mytb_dir + "/loader.sce");
b = csum6(2, 40);
assert_checkequal(b, 42);
