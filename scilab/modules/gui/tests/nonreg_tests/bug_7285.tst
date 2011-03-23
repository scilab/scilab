// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7285 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7285
//
// <-- Short Description -->
// Open/Save dialog box did not display filenames without extension with filter "*.*"
//
cd(TMPDIR);
mputl('blahblah',TMPDIR + '/test_bug_7285_1.sci');
mputl('blahblah',TMPDIR + '/test_bug_7285_2.sce');
mputl('blahblah',TMPDIR + '/test_bug_7285_3');

// please check that you see all files with selected extension "*.*"

f = uigetfile(["*.*";"*.sci";"*.sce"], TMPDIR, "file");

