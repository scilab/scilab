// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 8337 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8337
//
// <-- Short Description -->
// mtlb_rand() was affected by the seed of rand(), whereas it follow the uniform rule,
// as long as we don't have the 'rng' Matlab function to control the generation of random numbers.

rand("normal");
a = mtlb_rand(1000);

assert_checktrue(and(a>=0 & a<=1));
assert_checkequal(rand("info"), "normal");
