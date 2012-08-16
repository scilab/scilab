// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE <yann.collette@scilab.fr>
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4654
//
// <-- Short Description -->
// max of an empty sparse matrix makes Scilab hangs

[v, ij] = max(sprand(50,50,0));

assert_checkequal(v, 0);
assert_checkequal(ij, [1 1]);
