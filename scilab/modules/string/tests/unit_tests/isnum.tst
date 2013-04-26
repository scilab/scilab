// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checktrue(isnum('%pi'));

assert_checktrue(isnum('1'));
assert_checktrue(isnum('1.2'));
assert_checktrue(isnum('+1.2'));
assert_checktrue(isnum('-1.2'));

assert_checktrue(isnum('1e2'));
assert_checktrue(isnum('1d2'));
assert_checktrue(isnum('1E2'));
assert_checktrue(isnum('1D2'));

assert_checktrue(isnum('1e+2'));
assert_checktrue(isnum('1e-2'));
assert_checktrue(isnum('+1e+2'));
assert_checktrue(isnum('-1e+2'));

assert_checktrue(isnum('-12e+3'));

assert_checktrue(isnum('-1.2e+3'));
assert_checktrue(isnum('-1.2e+345'));

assert_checkfalse(isnum('a'));
assert_checkfalse(isnum('e'));
assert_checkfalse(isnum('d'));
assert_checkfalse(isnum('E'));
assert_checkfalse(isnum('e'));

assert_checkfalse(isnum('-1.2a+345'));
assert_checkfalse(isnum('-1.2e+3+45'));
assert_checkfalse(isnum('-1.2e+3-45'));

assert_checkerror("isnum(1)",[],999);
