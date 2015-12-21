// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7779 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7779
//
// <-- Short Description -->
// genlib did not check input arguments and did not use current path as default

DIR_TST = TMPDIR + "/BUG_7779";
mkdir(DIR_TST);
f = ['function toto()'; 'disp(''hello'')'; 'endfunction'];
mputl(f, DIR_TST + '/' + 'toto.sci');

cd(DIR_TST);
assert_checkequal(genlib('myLib'), %T);

msgerr = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), 'genlib', 1);
assert_checkerror ("genlib(1)", msgerr);

msgerr = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), 'genlib', 1);
assert_checkerror ("genlib([''myLib1'' ; ''myLib2''])", msgerr);

msgerr = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), 'genlib', 2);
assert_checkerror ("genlib(''myLib'', 1)", msgerr);

msgerr = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), 'genlib', 2);
assert_checkerror ("genlib(''myLib'', [''.'',''.''])", msgerr);
