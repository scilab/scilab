// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15647 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15647
//
// <-- Short Description -->
// spzeros(-1,-1) yields a corrupted result

errMsg = _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n")
assert_checkerror("spzeros(-1,-1)", msprintf(errMsg, "spzeros", 1));
assert_checkerror("spzeros(1,-1)", msprintf(errMsg, "spzeros", 2));
