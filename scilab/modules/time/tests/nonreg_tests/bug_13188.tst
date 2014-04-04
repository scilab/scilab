// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13188 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13118
//
// <-- Short Description -->
//
// The output argument of eomday was not initialized.

Y = [2011 2012 2013]';
M = [10 11 3]';
expected = [31 30 31]';
assert_checkequal(eomday(Y, M), expected);

Y = [2011 2012 2013];
M = [10 11 3];
expected = [31 30 31];
assert_checkequal(eomday(Y, M), expected);
