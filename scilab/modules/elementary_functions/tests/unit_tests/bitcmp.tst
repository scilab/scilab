// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit test for bitcmp function
// =============================================================================

// Tests for one input argument
assert_checkequal(bitcmp(uint8(99)),uint8(156));
assert_checkequal(bitcmp(uint16(99)),uint16(65436));
assert_checkequal(bitcmp(uint32(99)),uint32(4294967196));

assert_checkequal(bitcmp(uint8([13,99])),uint8([242,156]));
assert_checkequal(bitcmp(uint16([13,99])),uint16([65522,65436]));
assert_checkequal(bitcmp(uint32([13,99])),uint32([4294967282,4294967196]));

// Tests for two input arguments
assert_checktrue(bitcmp(99,8)==uint8(156));
assert_checktrue(bitcmp(99,16)==uint16(65436));
assert_checktrue(bitcmp(99,32)==uint32(4294967196));

assert_checktrue(bitcmp([13,99],8)==uint8([242,156]));
assert_checktrue(bitcmp([13,99],16)==uint16([65522,65436]));
assert_checktrue(bitcmp([13,99],32)==uint32([4294967282,4294967196]));

// Error messages
errmsg = msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),"bitcmp",1);
assert_checkerror("bitcmp()",errmsg); //no input argument
errmsg3 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"bitcmp",2);
assert_checkerror("bitcmp(99)",errmsg3); //First (and only) argument is not an unsigned integer
errmsg4 = msprintf(_("%s: Wrong input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitcmp",1);
assert_checkerror("bitcmp(""s"")",errmsg4); //input argument is a char
errmsg5 = msprintf(_("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"bitcmp",2,1,8);
assert_checkerror("bitcmp(uint8(99),16)",errmsg5); //wrong value for second input argument
errmsg6 = msprintf(_("%s: Wrong input argument #%d: An unsigned integer expected.\n"),"bitcmp",2);
assert_checkerror("bitcmp(uint8(99),5.5)",errmsg6); //second argument is a double
