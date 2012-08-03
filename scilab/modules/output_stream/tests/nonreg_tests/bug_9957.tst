// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9957 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9957
//
// <-- Short Description -->
// msprintf("%d\r", 2) returned '2r'
//

r1 = msprintf("%d\r", 12345);
ref1 = ascii([ 49.    50.    51.    52.    53.    13. ]);
assert_checkequal(r1, ref1);

r2 = msprintf("%d\r%d", 12345, 6789);
ref2 = ascii([ 49.    50.    51.    52.    53.    13.    54.    55.    56.    57.]);
assert_checkequal(r2, ref2);

r3 = msprintf("%d%s", 12345, "\r");
ref3 = "12345\r";
assert_checkequal(r3, ref3);
