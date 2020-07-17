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
// <-- Non-regression test for bug 16483 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16483
//
// <-- Short Description -->
// substraction of complex polynomial matrices is broken (Scilab 6 regression)

A=[%i+%s 0;0 %s];
B=[%i+%s 0;0 0];

assert_checkequal(A-B,[0 0;0 %s])
assert_checkequal(B-A,[0 0;0 -%s])
