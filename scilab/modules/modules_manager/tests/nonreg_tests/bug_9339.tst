// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for bug 9339 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9339
//
// <-- Short Description -->
// 'tbx_build_help' function did not manage languages correctly.

SRC = SCI + '/contrib/toolbox_skeleton';
DST = TMPDIR + '/toolbox_skeleton';
mkdir(DST);
assert_checkequal(isdir(DST), %T);
copyfile(SRC, DST);
cd(DST);
ilib_verbose(0);
exec('builder.sce');

assert_checkequal(isdir(DST + '/help/fr_FR/scilab_fr_FR_help'), %T);
assert_checkequal(isdir(DST + '/help/en_US/scilab_en_US_help'), %T);
