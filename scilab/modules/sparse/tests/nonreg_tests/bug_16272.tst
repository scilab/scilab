// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16272 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16272
//
// <-- Short Description -->
// spzeros(0,n) <> sparse([]) and spzeros(n,0) <> sparse([])

assert_checkequal(spzeros(0,5),sparse([]))
assert_checkequal(spzeros(5,0),sparse([]))
