// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 13041 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13041
//
// <-- Short Description -->
// Invalid result with int minus vect_int
//

b = int8(-9);
a = int8([-7 5]);
assert_checkequal(-a+b, b-a);

b = int16(-9);
a = int16([-7 5]);
assert_checkequal(-a+b, b-a);

b = int32(-9);
a = int32([-7 5]);
assert_checkequal(-a+b, b-a);

b = uint8(-9);
a = uint8([-7 5]);
assert_checkequal(-a+b, b-a);

b = uint16(-9);
a = uint16([-7 5]);
assert_checkequal(-a+b, b-a);

b = uint32(-9);
a = uint32([-7 5]);
assert_checkequal(-a+b, b-a);

