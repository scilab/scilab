// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9833 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9833
//
// <-- Short Description -->
// percent was bad formatted with Scierror function
//

ilib_verbose(0);
BUG_NAME = "bug_9833";

test_path = get_absolute_file_path(BUG_NAME + ".tst");

currentpath = pwd();

cd TMPDIR;
OS_TMP_DIR = pwd();


mkdir(OS_TMP_DIR, BUG_NAME);
TEST_DIR = OS_TMP_DIR + filesep() + BUG_NAME;

copyfile(SCI + "/modules/output_stream/tests/nonreg_tests/" + BUG_NAME + ".c" , TEST_DIR + filesep() + BUG_NAME + ".c");

chdir(TEST_DIR);

files = BUG_NAME + ".c";
ilib_build('lib' + BUG_NAME,["%" + BUG_NAME, "sci_" + BUG_NAME],files,[]);

// load the shared library 
exec loader.sce;

chdir(currentpath);

msgerr = msprintf(gettext("%s: Error.\n"), "%" + BUG_NAME);
assert_checkerror ("%" + BUG_NAME, msgerr);

// ulink() all libraries
ulink();

//remove TMP_DIR
rmdir(TEST_DIR,'s');
