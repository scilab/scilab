// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// Unitary tests of log2()
// =======================


// Base-2 mantissa + exponent decomposition:
// ----------------------------------------
[f, e] = log2([1, %pi, -3, %eps]);
if or(size(e) <> [1 4]) then pause, end
if or(size(f) <> [1 4]) then pause, end
if or(e <> [1.    2.    2.  -51.]) then pause, end

// Checking error messages
msg = gettext("%s: Wrong number of input argument(s): %d expected.\n");
assert_checkerror("[f, e] = log2();", msprintf(msg, "log2", 1));
msg = gettext("%s: Argument #%d: Decimal numbers expected.\n");
assert_checkerror("[f, e] = log2(""a"");", msprintf(msg, "log2", 1));
msg = gettext("%s: Argument #%d: Complex numbers not supported.\n");
assert_checkerror("[f, e] = log2(%i);", msprintf(msg, "log2", 1));
