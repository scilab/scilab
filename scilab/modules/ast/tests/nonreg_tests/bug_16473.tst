// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16473 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16473
//
// <-- Short Description -->
// Deleting rows with [] insertion in a sparse squares the matrix with padding zeros (6.0.0 regression)

m = int(sprand(10,3,0.5)*10);
m([2 5],:) = [];
assert_checkequal(size(m),[8 3])