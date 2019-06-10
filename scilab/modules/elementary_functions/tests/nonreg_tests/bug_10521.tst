// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10521 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10521
//
// <-- Short Description -->
// [f,e] = log2(x) returned an error
//

[f, e] = log2([1,%pi,-3,%eps]);
e_ref = [1.    2.    2.  -51.];
assert_checkequal(e, e_ref);
