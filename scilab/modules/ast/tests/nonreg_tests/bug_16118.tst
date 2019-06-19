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
// <-- Non-regression test for bug 16118 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16118
//
// <-- Short Description -->
// polynomials: %z <> (1+%z) returns %F (same with ~=) (6.0.0 regression)

assert_checkequal(%s <> 1+%s, %t)
assert_checkequal(%s <> [1+%s, %s], [%t, %f])
assert_checkequal(1+%s <> %s, %t)
assert_checkequal([1+%s, %s] <> %s, [%t, %f])

assert_checkequal([%s, 1+%s] <> [1+%s, %s], [%t, %t])
assert_checkequal([1+%s, 1+%s] <> [1+%s, %s], [%f, %t])
assert_checkequal([1+%s, %s] <> [1+%s, %s], [%f, %f])

