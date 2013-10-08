// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 3639 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3639
//
// <-- Short Description -->
// link without parameters can crash

ilib_verbose(0);

currentpath = pwd();
cd TMPDIR;
OS_TMP_DIR = pwd();

TEST_NAME = 'bug_3639_1';
mkdir(OS_TMP_DIR, TEST_NAME);
TEST_DIR_1 = OS_TMP_DIR + filesep() + TEST_NAME;
copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug_3639.c' , TEST_DIR_1 + filesep() + 'bug_3639.c');
chdir(TEST_DIR_1);

files = ['bug_3639.c'];
ilib_build('libc_fun1', ['c_sum1','c_intsum'; 'c_sub1', 'c_intsub'], files, []);
// load the shared library 
info_link = link();
assert_checkequal(info_link, []);
exec loader.sce;

TEST_NAME = 'bug_3639_2';
mkdir(OS_TMP_DIR, TEST_NAME);
TEST_DIR_2 = OS_TMP_DIR + filesep() + TEST_NAME;
copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug_3639.c' , TEST_DIR_2 + filesep() + 'bug_3639.c');
chdir(TEST_DIR_2);

ilib_build('libc_fun2', ['c_sum2', 'c_intsum'; 'c_sub2','c_intsub'], files, []);
info_link = link();
assert_checkequal(info_link, 'libc_fun1');
exec loader.sce;

info_link = link();
assert_checkequal(info_link, ['libc_fun2','libc_fun1']);

// cleaning
chdir(currentpath);
// ulink() all libraries
ulink();
clearfun('c_sum1');
clearfun('c_sub1');
clearfun('c_sum2');
clearfun('c_sub2');

//remove TMP_DIR
rmdir(TEST_DIR_1,'s');
rmdir(TEST_DIR_2,'s');
// =============================================================================
