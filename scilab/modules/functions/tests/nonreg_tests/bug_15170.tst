// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15170 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15170
//
// <-- Short Description -->
// getd() generated a variable k that could overwrite a loaded one.

d = TMPDIR+"\getd_tmp";
mkdir(d);
f = [
"function r = test_getd()"
" r = 1;"
"endfunction"
"k = %pi;"
];
mputl(f, d+"\test_getd.sci");
getd(d);
assert_checktrue(isdef("test_getd","l"));
assert_checkfalse(isdef("k_getd_loop","l"));
assert_checktrue(isdef("k","l"));
assert_checkequal(k, %pi);
rmdir(d,"s");
