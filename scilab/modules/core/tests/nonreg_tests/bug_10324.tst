// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10324 -->
//
// <-- CLI SHELL MODE -->
//
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10324
//
// <-- Short Description -->
// format(m) where m is a vector did not work but the documentation states it can be used.


v = format();
m = [v(2) v(1)];
format(m);
f = format();
assert_checkequal(v, f);

v = format();
m = [v(2);v(1)];
format(m);
f = format();
assert_checkequal(v, f);


m = [10 1 2];
msgerr = msprintf(gettext("%s: Wrong size for input argument #%d.\n"),"format", 1);
assert_checkerror ( "format(m)" , msgerr );