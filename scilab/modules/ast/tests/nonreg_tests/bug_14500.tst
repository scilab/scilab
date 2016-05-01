// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14500 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14500
//
// <-- Short Description -->
// Operator ".^" was broken for sparse matrices.

r = sprand(100, 100, 0.001);
r.^2; // This line should not segfault
