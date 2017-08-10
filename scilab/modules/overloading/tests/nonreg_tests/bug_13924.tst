//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 13924 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13924
//
// <-- Short Description -->
// rationals: r1==r2 and r1~=r2 may be wrong and operate unconsistenly.

a = %z/(1-%z);
b = -%z / -(1-%z);
assert_checkequal(a,b)

assert_checkequal(size([a b]==[a a], "*"), 2)
assert_checkequal(size([a b]~=[a a], "*"), 2)

// Disable simplification mode
simp_mode(%f)

a = %z/(1-%z);
b = -%z / -(1-%z);
c = 0 / (1-%z);
d = 0 / (2+%z);

assert_checkequal(a,b)
assert_checkequal(c,d)

assert_checkequal(size([a b c d]==[a a c c], "*"), 4)
assert_checkequal(size([a b c d]~=[a a c c], "*"), 4)
