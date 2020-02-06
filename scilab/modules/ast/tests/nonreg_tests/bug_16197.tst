// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16197 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16197
//
// <-- Short Description -->
// Extraction of real components of complex matrices is counterintuitive

x = [1+%i 1 2 3 1-%i];
s = x * %s;

//single index
//double
assert_checkfalse(isreal(x(1)));
assert_checktrue(isreal(x(2)));
assert_checktrue(isreal(x(3)));
assert_checktrue(isreal(x(4)));
assert_checkfalse(isreal(x(5)));

//poly
assert_checkfalse(isreal(s(1)));
assert_checktrue(isreal(s(2)));
assert_checktrue(isreal(s(3)));
assert_checktrue(isreal(s(4)));
assert_checkfalse(isreal(s(5)));

//implicit list
//double
assert_checkfalse(isreal(x(1:4:5)));
assert_checktrue(isreal(x(2:4)));

//poly
assert_checkfalse(isreal(s(1:4:5)));
assert_checktrue(isreal(s(2:4)));

//implicit list > 1 dim
//double
assert_checkfalse(isreal(x(1, 1:4:5)));
assert_checktrue(isreal(x(1, 2:4)));

//poly
assert_checkfalse(isreal(s(1:4:$)));
assert_checktrue(isreal(s(2:$-1)));

//Matrix index
//double
assert_checkfalse(isreal(x([1 5])));
assert_checktrue(isreal(x([2 3 4])));

//poly
assert_checkfalse(isreal(s([1 5])));
assert_checktrue(isreal(s([2 3 4])));

