// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 14499 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14499
//
// <-- Short Description -->
// getd() did not update functions already defined
fp = funcprot(0);
d = TMPDIR+"\getd_tmp";
mkdir(d);
f = [
"function r = test_getd()"
" r = 1;"
"endfunction"
];
mputl(f, d+"\test_getd.sci");
getd(d);
r = test_getd();
f(2) = " r = 2;";
mputl(f, d+"\test_getd.sci");
getd(d);
r = test_getd();
assert_checkequal(r,2);
rmdir(d,"s");
funcprot(fp);
