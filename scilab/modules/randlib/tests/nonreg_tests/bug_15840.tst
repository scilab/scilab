// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15840 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15840
//
// <-- Short Description -->
// grand(1,"prm",m) yields an unsqueezed size([size(m) 1]) hypermatrix (Regression)
m = rand(2,3);
m = grand(1,"prm",m);
assert_checkequal(ndims(m),2);
assert_checkequal(size(m),[2 3]);