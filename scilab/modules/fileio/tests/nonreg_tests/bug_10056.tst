// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10056 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10056
//
// <-- Short Description -->
// segfault with mgetl


a="";
for i=1:8193
    a=a + "a";
end;

mputl(a, TMPDIR + "/bug_13721.txt");
b = mgetl(TMPDIR + "/bug_13721.txt");
assert_checkequal(a, b);
