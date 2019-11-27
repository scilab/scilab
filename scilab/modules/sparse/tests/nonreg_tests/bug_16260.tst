// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16260 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16260
//
// <-- Short Description -->
// nnz(): please reopen it to overloading (Regression), and extend it to dense boolean and polynomial arrays

assert_checkequal(nnz([%t %f]),1)
assert_checkequal(nnz([%t %t;%f %f]),2)
assert_checkequal(nnz([%s 0]),1)
assert_checkequal(nnz([%s %s;0 0]),2)

