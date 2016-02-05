// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13838 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13838
//
// <-- Short Description -->
// Sparse and complexe substraction made Scilab crash.

a = sparse(rand(5,2));
a(8) = a(8) - %i;
a(8) = a(8) - 1;