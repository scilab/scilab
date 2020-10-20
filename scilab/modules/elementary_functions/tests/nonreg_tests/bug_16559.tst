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
// <-- Non-regression test for bug 16559 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16559
//
// <-- Short Description -->
// A(:,:) is empty for sparse matrix of dimension 2^16 or larger

n = 2^16;
A = speye(n,n);
assert_checkequal(size(A,"*"),n*n);
assert_checkfalse(isempty(A));