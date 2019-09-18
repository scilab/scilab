// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14746 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14746
//
// <-- Short Description -->
// The display of tiny complex numbers displays 0

format("v")
disp([1, %eps/2])
assert_checkequal(string([1, %eps/2]) , ["1","1.110D-16"]);

format("e")
disp([1, %eps/2])
assert_checkequal(string([1, %eps/2]) , ["1.000D+00","1.110D-16"]);

format("v")
disp(complex(%eps/2,0))
assert_checkequal(string(complex(%eps/2,0)) , "1.110D-16");

format("e")
disp(complex(%eps/2,0))
assert_checkequal(string(complex(%eps/2,0)) , "1.110D-16");

