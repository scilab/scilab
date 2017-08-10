//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Scilab Enterprises - Alexandre HERISSE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12296 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12296
//
// <-- Short Description -->
// call to getenv with an big environment variable leads to memory corruption

a = "azerty"
b = "";
for i = 1:4000
    b = b + a;
end

assert_checktrue(setenv("test", b));
assert_checkequal(getenv("test"), b);

