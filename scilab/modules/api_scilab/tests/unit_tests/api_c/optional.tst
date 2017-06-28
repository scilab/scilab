// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->

ilib_verbose(0);

tmpDir = fullfile(TMPDIR, "api_c");
mkdir(tmpDir);
cd(tmpDir);
copyfile(fullfile(SCI, "modules/api_scilab/tests/unit_tests/api_c/optional.c"), ..
    fullfile(tmpDir, "optional.c"));

ilib_build("liboptional", ["optional_args", "sci_optional_args"], "optional.c", ..
    [], "", "", "");
exec("loader.sce");

ret = optional_args();
assert_checkequal(ret, "John Doe, 77 years old, does not have a driving license.");
ret = optional_args(name="Bill");
assert_checkequal(ret, "Bill, 77 years old, does not have a driving license.");
ret = optional_args(name="Bill", age=22);
assert_checkequal(ret, "Bill, 22 years old, does not have a driving license.");
ret = optional_args(age=22, name="Bill");
assert_checkequal(ret, "Bill, 22 years old, does not have a driving license.");
ret = optional_args(name="Bill", age=22, driving_license=%t);
assert_checkequal(ret, "Bill, 22 years old, has a driving license.");
ret = optional_args(name="Walter", driving_license=%t);
assert_checkequal(ret, "Walter, 77 years old, has a driving license.");

ulink(0);