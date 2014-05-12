// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11405 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11405
//
// <-- Short Description -->
// Added hypermatrix support for extraction.

vec = 0:4;
indexes = matrix([1 1; 1 1; 3 3; 3 3], [2 2 2]);
// Staying consistent with the way other extractions currently work,
// so equivalent to vec(indexes(:)).
assert_checkequal(vec(indexes), [0 0 2 2 0 0 2 2]);

vec = [vec; vec];
// Equivalent to vec(indexes(:)).
assert_checkequal(vec(indexes), [0;0;1;1;0;0;1;1]);
