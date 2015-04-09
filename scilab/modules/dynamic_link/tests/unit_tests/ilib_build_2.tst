// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

if getos() == "Windows" then
    TMP_OS_DIR = getenv("TMP","err");
    if (TMP_OS_DIR == "err") then pause,end;
else
    TMP_OS_DIR = "/tmp";
    if ~isdir(TMP_OS_DIR) then pause,end;
end

cur_dir = pwd();

TEST_NAME = "ilib_build_2";
mkdir(TMP_OS_DIR, TEST_NAME);
TEST_DIR_1 = TMP_OS_DIR + filesep() + TEST_NAME;
copyfile(SCI+"/modules/dynamic_link/tests/unit_tests/sci_csum.c" , TEST_DIR_1 + filesep() + "sci_csum.c");
copyfile(SCI+"/modules/dynamic_link/tests/unit_tests/sci_csub.c" , TEST_DIR_1 + filesep() + "sci_csub.c");
chdir(TEST_DIR_1);

files=["sci_csub.c","sci_csum.c"];
ilib_build("foo",["c_sum","sci_csum";"c_sub","sci_csub"],files,[]);


// load the shared library
exec loader.sce;

if c_sum(3,5) <> 8 then pause,end
if c_sub(3,5) <> -2 then pause,end

chdir(cur_dir);

// ulink() all libraries
ulink();


//remove TMP_DIR
rmdir(TEST_DIR_1,"s");
