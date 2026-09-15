// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/9339
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

assert_checktrue(isdir(DST + '/help/images'));
assert_checktrue(isfile(DST + '/help/images/c_sub_2.png'));
assert_checktrue(isfile(DST + '/help/images/c_sub_fr_FR_2.png'));
assert_checktrue(isfile(DST + '/inline/en_US/links.json'));
assert_checktrue(isfile(DST + '/inline/fr_FR/links.json'));
