// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15285 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15285
//
// <-- Short Description -->
//resize_matrix does not manage convertion to int64 and uint64

a = rand(2, 3) * 1000;
ref = zeros(5, 5);
ref(1:2, 1:3) = a;
ref64 = int64(ref);
refu64 = uint64(ref);
assert_checkequal(resize_matrix(a, [5, 5], "int64"), ref64);
assert_checkequal(resize_matrix(a, [5, 5], "uint64"), refu64);
