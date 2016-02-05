// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009-2011 - DIGITEO
// Copyright (C) 2015 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// =============================================================================
//Here with give a complete example on adding new primitive to Scilab
ilib_verbose(0);

currentpath = pwd();
cd TMPDIR;
OS_TMP_DIR = pwd();

TEST_NAME = "ilib_build_cpp";
mkdir(OS_TMP_DIR, TEST_NAME);
TEST_DIR_1 = OS_TMP_DIR + filesep() + TEST_NAME;
copyfile(SCI+"/modules/dynamic_link/tests/unit_tests/sci_cppfind.cxx" , TEST_DIR_1 + filesep() + "sci_cppfind.cxx");
chdir(TEST_DIR_1);

//creating the shared library (a gateway, a Makefile and a loader are
//generated.

files = ["sci_cppfind.cxx"];
ilib_build("foo", ["cppfind", "sci_cppfind"], files,[]);

// load the shared library
exec loader.sce;

// Small test to see if the function is actually working.
assert_checkequal(cppfind("my very long string", "long"), 8);
assert_checkequal(cppfind("my very long string","very"), 3);
assert_checkequal(cppfind("my very long string","short"), -1);

if getos() == "Linux" | getos() == "Darwin" then

    // Force the usage
    files = ["sci_cppfind.cxx"];

    ilib_build("foo2", ["cppfind2", "sci_cppfind"], files,[],[], [], "-fpermissive", [], [], "g++");
    exec loader.sce;

    // Small test to see if the function is actually working.
    assert_checkequal(cppfind("my very long string", "long"), 8);
    assert_checkequal(cppfind("my very long string","very"), 3);
    assert_checkequal(cppfind("my very long string","short"), -1);

end
