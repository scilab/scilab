// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15808 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15808
//
// <-- Short Description -->
// [5i] is parsed as [5,i]

clear i
assert_checkerror("5i",["5i";"^~^";"Error: syntax error, unexpected identifier, expecting end of file"])
assert_checkerror("[5i]",["[5i]";" ^~^";"Error: syntax error, unexpected identifier, expecting end of file"])
assert_checkerror("[5 i]","Undefined variable: i")
assert_checkerror("[5,i]","Undefined variable: i")
i = 1
assert_checkequal([5 i],[5,1])
assert_checkequal([5,i],[5,1]) 