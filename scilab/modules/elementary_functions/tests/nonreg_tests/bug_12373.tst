// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12373 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12373
//
// <-- Short Description -->
// pmodulo(n,m<0) returned a negative result
//

assert_checkequal(pmodulo(10,-6), 4);
assert_checkequal(pmodulo(-10,-6), 2);

assert_checkfalse(pmodulo(10,-6) == -2);
assert_checkfalse(pmodulo(-10,-6) == -4);