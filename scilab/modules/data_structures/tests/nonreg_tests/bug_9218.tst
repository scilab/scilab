// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9218 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9218
//
// <-- Short Description -->
// a("b") = 10 makes Scilab crash
// but a = struct(); a("b") = 10 works.

clear a
a("b") = 10;
assert_checkequal(a.b, 10);

clear a
a.b = 10;
assert_checkequal(a.b, 10);

clear a
a = struct();
a("b") = 10;
assert_checkequal(a.b, 10);
