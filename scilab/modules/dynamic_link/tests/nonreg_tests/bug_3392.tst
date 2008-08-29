// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3392 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3392
//

test_path = get_absolute_file_path('bug_3392.tst');

currentpath = pwd();

cd TMPDIR;
cd ../;
OS_TMP_DIR = pwd();


mkdir(OS_TMP_DIR,'bug_3392');
TEST_DIR = OS_TMP_DIR + filesep() + 'bug_3392';

copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug_3392.c' , TEST_DIR + filesep() + 'bug_3392.c');

chdir(TEST_DIR);

files=['bug_3392.o'];
ilib_build('libc_fun',['c_sum','c_intsum';'c_sub','c_intsub'],files,[]);

// disable message
warning_mode = warning('query');
warning('off');

// load the shared library 
exec loader.sce

// enable message 
warning(warning_mode);

chdir(currentpath);

//using the new primitive
if c_sum(3,8) <> 3+8 then pause,end
if c_sub(3,8) <> 3-8 then pause,end

// ulink() all libraries
ulink();

//remove TMP_DIR
rmdir(TEST_DIR,'s');
// =============================================================================
