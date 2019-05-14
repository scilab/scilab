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
// <-- Non-regression test for bug 16085 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16085
//
// <-- Short Description -->
// insertion in an empty struct is broken (regression)

x=struct();
y.b = 1;
y.c = 2;
x(:) = y;

assert_checktrue(isstruct(x));
assert_checkequal(fieldnames(x),["b";"c"])
assert_checkequal(x.b,1);
assert_checkequal(x.c,2);

