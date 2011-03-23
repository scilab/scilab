// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4224 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4224
//
// <-- Short Description -->
// uigetfile masks were case insensitive on Windows.

cd(TMPDIR);
mputl('blahblah',TMPDIR + '/test_bug_4224.sci');
mputl('blahblah',TMPDIR + '/test_bug_4224.sci.sCi');
mputl('blahblah',TMPDIR + '/test_bug_4224_1.SCI');
mputl('blahblah',TMPDIR + '/test_bug_4224_2.ScI');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.Sci');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.sCI');

mputl('blahblah',TMPDIR + '/test_bug_4224.pol');
mputl('blahblah',TMPDIR + '/test_bug_4224.pol.pol');
mputl('blahblah',TMPDIR + '/test_bug_4224_1.POL');
mputl('blahblah',TMPDIR + '/test_bug_4224_2.PoL');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.Pol');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.pOL');

mputl('blahblah',TMPDIR + '/test_bug_4224.filt');
mputl('blahblah',TMPDIR + '/test_bug_4224.filt.filt');
mputl('blahblah',TMPDIR + '/test_bug_4224_1.FILT');
mputl('blahblah',TMPDIR + '/test_bug_4224_2.FiLT');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.Filt');
mputl('blahblah',TMPDIR + '/test_bug_4224_3.fILT');

// please check that you see all files with selected extension on Windows.
// and that on others platforms only case sensitive extensions.

f = uigetfile(["*.filt";"*.pol";"*.sci"],TMPDIR,"file");


