// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8391
//
// <-- Short Description -->
// a(1:10, 3:$) = [] resized "a" instead of trimming it.

a = ["1.1", "1.2", "1.3", "1.4"
"2.1", "2.2", "2.3", "2.4"
"3.1", "3.2", "3.3", "4.4"
"4.1", "4.2", "3.3", "4.4"
];
a(1:10, 3:$) = [];
expected = [
"1.1", "1.2"
"2.1", "2.2"
"3.1", "3.2"
"4.1", "4.2"
];
assert_checkequal(a, expected);
