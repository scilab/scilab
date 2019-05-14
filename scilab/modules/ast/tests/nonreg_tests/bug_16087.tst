// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16087 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16087
//
// <-- Short Description -->
// Insertion of struct() in a non-empty struct crashes Scilab

x.a = 1;
x.b = 2;
y = x;
x(1) = struct();

assert_checkequal(x,y)

