// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 13515 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13515
//
// <-- Short Description -->
// there were wrong results for matrix/hypermatrix with bitset function
//
// <-- CLI SHELL MODE -->

clear "checkBitset"
function checkBitset(x, pos, v, isOne, typeOfx)
    if isOne then
        select typeOfx
        case 0 // double
            assert_checkequal(bitset(x, pos), dbRefOne);
            assert_checkequal(bitset(x, pos, v), dbRefOne);
        case 8 // uint8
            assert_checkequal(bitset(x, pos), uint8RefOne);
            assert_checkequal(bitset(x, pos, v), uint8RefOne);
        case 16 // uint16
            assert_checkequal(bitset(x, pos), uint16RefOne);
            assert_checkequal(bitset(x, pos, v), uint16RefOne);
        case 32 // uint32
            assert_checkequal(bitset(x, pos), uint32RefOne);
            assert_checkequal(bitset(x, pos, v), uint32RefOne);
        end
    else
        select typeOfx
        case 0
            assert_checkequal(bitset(x, pos, v), dbRefZero);
        case 8
            assert_checkequal(bitset(x, pos, v), uint8RefZero);
        case 16
            assert_checkequal(bitset(x, pos, v), uint16RefZero);
        case 32
            assert_checkequal(bitset(x, pos, v), uint32RefZero);
        end
    end
endfunction

clear "createValues"
function [dbValue, dbPos, dbBitValue, ...
    uint8Value, uint8Pos, uint8BitValue, ...
    uint16Value, uint16Pos, uint16BitValue, ...
    uint32Value, uint32Pos, uint32BitValue] = createValues(value, pos, bitValue)
    // double
    dbValue = value;
    dbPos = pos;
    dbBitValue = bitValue
    // uint8
    uint8Value = uint8(value);
    uint8Pos = uint8(pos);
    uint8BitValue = uint8(bitValue);
    // uint16
    uint16Value = uint16(value);
    uint16Pos = uint16(pos);
    uint16BitValue = uint16(bitValue);
    // uint32
    uint32Value = uint32(value);
    uint32Pos = uint32(pos);
    uint32BitValue = uint32(bitValue);
endfunction

clear "createReference";
function [dbRefOne, dbRefZero, ...
    uint8RefOne, uint8RefZero, ...
    uint16RefOne, uint16RefZero, ...
    uint32RefOne, uint32RefZero] = createReference(refOne, refZero)
    dbRefOne = refOne;
    dbRefZero = refZero;
    uint8RefOne = uint8(dbRefOne);
    uint16RefOne = uint16(dbRefOne);
    uint32RefOne = uint32(dbRefOne);
    uint8RefZero = uint8(dbRefZero);
    uint16RefZero = uint16(dbRefZero);
    uint32RefZero = uint32(dbRefZero);
endfunction

// check input arguments
lstr = gettext("%s: Wrong number of input argument(s): At least %d expected.\n");
errmsg = msprintf (lstr, "bitset", 2);
assert_checkerror("bitset(1)", errmsg);
assert_checkerror("bitset()", errmsg);
lstr = gettext("%s: Wrong size for input arguments: Same sizes expected.\n");
errmsg = msprintf (lstr, "bitset");
assert_checkerror("bitset([], [1, 2])", errmsg);
lstr = gettext("%s: Wrong value for input argument #%d: 0 or 1 expected.\n");
errmsg = msprintf (lstr, "bitset", 3);
assert_checkerror("bitset(1, 1, 3)", errmsg);
assert_checkerror("bitset(1, 1, %t)", errmsg);
assert_checkerror("bitset([1, 2], [1, 1], [2, 0])", errmsg);
assert_checkerror("bitset([1; 2; 3], [1; 1; 1], [0; 0; 4])", errmsg);
lstr = gettext("%s: Wrong size for input arguments: Same sizes expected.\n");
errmsg = msprintf (lstr, "bitset");
assert_checkerror("bitset(1, [1, 2], 1)", errmsg);
assert_checkerror("bitset([1, 2], [1, 2, 3], 1)", errmsg);
assert_checkerror("bitset([1, 2, 3], [1, 2, 3], 1)", errmsg);
lstr = gettext("%s: Wrong type for input argument #%d: Scalar/matrix/hypermatrix of unsigned integers expected.\n");
errmsg = msprintf (lstr, "bitset", 1);
assert_checkerror("bitset(""1"", 1, 1)", errmsg);
assert_checkerror("bitset(poly(0,""s""), 1, 1)", errmsg);
assert_checkerror("bitset([1, -1], [1, 0], [1, 0])", errmsg);
assert_checkerror("bitset([0.5, 0.2], [1, 0], [1, 0])", errmsg);
assert_checkerror("bitset([%t, %t], [1, 0], [1, 0])", errmsg);
assert_checkerror("bitset(int32([1, 1]), [1, 0], [1, 0])", errmsg);
assert_checkerror("bitset(int16([1, 1]), [1, 0], [1, 0])", errmsg);
assert_checkerror("bitset(int8([1, 1]), [1, 0], [1, 0])", errmsg);
errmsg = msprintf (lstr, "bitset", 2);
assert_checkerror("bitset(1,""1"", 1)", errmsg);
assert_checkerror("bitset(1, poly(0,""s""), 1)", errmsg);
assert_checkerror("bitset([1, 0], [1, -1], [1, 0])", errmsg);
assert_checkerror("bitset([1, 0], [0.5, 0.2], [1, 0])", errmsg);
assert_checkerror("bitset([1; 0], [%t; %t], [1; 0])", errmsg);
assert_checkerror("bitset([1, 0], int32([1, 1]), [1, 0])", errmsg);
assert_checkerror("bitset([1, 0], int16([1, 1]), [1, 0])", errmsg);
assert_checkerror("bitset([1, 0], int8([1, 1]), [1, 0])", errmsg);
lstr = gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n");
errmsg = msprintf (lstr, "bitset", 2, 1, 52);
assert_checkerror("bitset([1, 0], [1, 53], [1, 0])", errmsg);
assert_checkerror("bitset([1, 0], [0, 5], [1, 0])", errmsg);
errmsg = msprintf (lstr, "bitset", 2, 1, 8);
assert_checkerror("bitset(uint8([1, 0; 1, 0]), [1, 2; 1, 9], [1, 0; 1, 1])", errmsg);
assert_checkerror("bitset(uint8([1, 0; 1, 0]), [1, 0; 0, 1], [1, 0; 1, 1])", errmsg);
errmsg = msprintf (lstr, "bitset", 2, 1, 16);
assert_checkerror("bitset(uint16([1, 0; 1, 0]), [1, 17; 1, 9], [1, 0; 1, 1])", errmsg);
assert_checkerror("bitset(uint16([1, 0; 1, 0]), [1, 0; 0, 1], [1, 0; 1, 1])", errmsg);
errmsg = msprintf (lstr, "bitset", 2, 1, 32);
assert_checkerror("bitset(uint32([1, 0; 1, 0]), [1, 17; 33, 1], [1, 0; 1, 1])", errmsg);
assert_checkerror("bitset(uint32([1, 0; 1, 0]), [1, 1; 0, 1], [1, 0; 1, 1])", errmsg);

// check results
// create value to check
value = [9, 10, 11, 12, 13, 14, 15];
position = [2, 3, 4, 5, 6, 7, 8];
bitValue = zeros(1:7);
[dbValue, dbPos, dbBitValue, ...
uint8Value, uint8Pos, uint8BitValue, ...
uint16Value, uint16Pos, uint16BitValue, ...
uint32Value, uint32Pos, uint32BitValue] = createValues(value, position, bitValue);

// create references
refOne = [11, 14, 11, 28, 45, 78, 143];
refZero = [9, 10, 3, 12, 13, 14, 15];
[dbRefOne, dbRefZero, ...
uint8RefOne, uint8RefZero, ...
uint16RefOne, uint16RefZero, ...
uint32RefOne, uint32RefZero] = createReference(refOne, refZero);

// check only the same types
// only double
checkBitset(dbValue, dbPos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, dbPos, dbBitValue, %f, 0);
// only uint8
checkBitset(uint8Value, uint8Pos, uint8BitValue + 1, %t, 8);
checkBitset(uint8Value, uint8Pos, uint8BitValue, %f, 8);
// only uint16
checkBitset(uint16Value, uint16Pos, uint16BitValue + 1, %t, 16);
checkBitset(uint16Value, uint16Pos, uint16BitValue, %f, 16);
// only uint32
checkBitset(uint32Value, uint32Pos, uint32BitValue + 1, %t, 32);
checkBitset(uint32Value, uint32Pos, uint32BitValue, %f, 32);

// check mixte types
// double
checkBitset(dbValue, uint8Pos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, uint8Pos, dbBitValue, %f, 0);
checkBitset(dbValue, uint8Pos, uint8BitValue, %f, 0);
checkBitset(dbValue, uint16Pos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, uint16Pos, dbBitValue, %f, 0);
checkBitset(dbValue, uint16Pos, uint8BitValue, %f, 0);
checkBitset(dbValue, uint32Pos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, uint32Pos, dbBitValue, %f, 0);
// uint8
checkBitset(uint8Value, dbPos, dbBitValue + 1, %t, 8);
checkBitset(uint8Value, dbPos, dbBitValue, %f, 8);
checkBitset(uint8Value, uint16Pos, uint16BitValue + 1, %t, 8);
checkBitset(uint8Value, uint16Pos, dbBitValue, %f, 8);
checkBitset(uint8Value, uint32Pos, uint8BitValue, %f, 8);
checkBitset(uint8Value, uint32Pos, dbBitValue + 1, %t, 8);
// uint16
checkBitset(uint16Value, dbPos, dbBitValue + 1, %t, 16);
checkBitset(uint16Value, dbPos, dbBitValue, %f, 16);
checkBitset(uint16Value, uint8Pos, uint16BitValue + 1, %t, 16);
checkBitset(uint16Value, uint8Pos, dbBitValue, %f, 16);
checkBitset(uint16Value, uint32Pos, uint8BitValue, %f, 16);
checkBitset(uint16Value, uint32Pos, dbBitValue + 1, %t, 16);
// uint32
checkBitset(uint32Value, dbPos, uint32BitValue + 1, %t, 32);
checkBitset(uint32Value, dbPos, uint32BitValue, %f, 32);
checkBitset(uint32Value, uint16Pos, uint16BitValue + 1, %t, 32);
checkBitset(uint32Value, uint16Pos, dbBitValue, %f, 32);
checkBitset(uint32Value, uint8Pos, uint8BitValue, %f, 32);
checkBitset(uint32Value, uint8Pos, dbBitValue + 1, %t, 32);


// create value to check
value = [4; 5; 6];
position = [1; 2; 3];
bitValue = zeros(1:3)';
[dbValue, dbPos, dbBitValue, ...
uint8Value, uint8Pos, uint8BitValue, ...
uint16Value, uint16Pos, uint16BitValue, ...
uint32Value, uint32Pos, uint32BitValue] = createValues(value, position, bitValue);

// create references
refOne = [5; 7; 6];
refZero = [4; 5; 2];
[dbRefOne, dbRefZero, ...
uint8RefOne, uint8RefZero, ...
uint16RefOne, uint16RefZero, ...
uint32RefOne, uint32RefZero] = createReference(refOne, refZero);

// check values
// double
checkBitset(dbValue, dbPos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, dbPos, dbBitValue, %f, 0);
// uint8
checkBitset(uint8Value, uint8Pos, uint8BitValue + 1, %t, 8);
checkBitset(uint8Value, uint8Pos, uint8BitValue, %f, 8);
// uint16
checkBitset(uint16Value, uint16Pos, uint16BitValue + 1, %t, 16);
checkBitset(uint16Value, uint16Pos, uint16BitValue, %f, 16);
// uint32
checkBitset(uint32Value, uint32Pos, uint32BitValue + 1, %t, 32);
checkBitset(uint32Value, uint32Pos, uint32BitValue, %f, 32);

// create value to check
value = [1, 2, 3; 4, 5, 6];
position = [2, 2, 2; 3, 3, 3];
bitValue = zeros(2, 3);
[dbValue, dbPos, dbBitValue, ...
uint8Value, uint8Pos, uint8BitValue, ...
uint16Value, uint16Pos, uint16BitValue, ...
uint32Value, uint32Pos, uint32BitValue] = createValues(value, position, bitValue);

// create references
refOne = [3, 2, 3; 4, 5, 6];
refZero = [1, 0, 1; 0, 1, 2];
[dbRefOne, dbRefZero, ...
uint8RefOne, uint8RefZero, ...
uint16RefOne, uint16RefZero, ...
uint32RefOne, uint32RefZero] = createReference(refOne, refZero);

// check values
// double
checkBitset(dbValue, dbPos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, dbPos, dbBitValue, %f, 0);
// uint8
checkBitset(uint8Value, uint8Pos, uint8BitValue + 1, %t, 8);
checkBitset(uint8Value, uint8Pos, uint8BitValue, %f, 8);
// uint16
checkBitset(uint16Value, uint16Pos, uint16BitValue + 1, %t, 16);
checkBitset(uint16Value, uint16Pos, uint16BitValue, %f, 16);
// uint32
checkBitset(uint32Value, uint32Pos, uint32BitValue + 1, %t, 32);
checkBitset(uint32Value, uint32Pos, uint32BitValue, %f, 32);

// create value to check
value = matrix(1:12, [2 3 1 2]);
position = ones(value)*3;
position(1:6) = 4;
bitValue = matrix(zeros(1:12), size(value));
[dbValue, dbPos, dbBitValue, ...
uint8Value, uint8Pos, uint8BitValue, ...
uint16Value, uint16Pos, uint16BitValue, ...
uint32Value, uint32Pos, uint32BitValue] = createValues(value, position, bitValue);

// create references
refOne = [9, 10, 11, 12, 13, 14, 7, 12, 13, 14, 15, 12];
refZero = [1, 2, 3, 4, 5, 6, 3, 8, 9, 10, 11, 8];
refOne = matrix(refOne, [2 3 1 2]);
refZero = matrix(refZero, [2 3 1 2]);
[dbRefOne, dbRefZero, ...
uint8RefOne, uint8RefZero, ...
uint16RefOne, uint16RefZero, ...
uint32RefOne, uint32RefZero] = createReference(refOne, refZero);

// check values
// double
checkBitset(dbValue, dbPos, dbBitValue + 1, %t, 0);
checkBitset(dbValue, dbPos, dbBitValue, %f, 0);
// uint8
checkBitset(uint8Value, uint8Pos, uint8BitValue + 1, %t, 8);
checkBitset(uint8Value, uint8Pos, uint8BitValue, %f, 8);
// uint16
checkBitset(uint16Value, uint16Pos, uint16BitValue + 1, %t, 16);
checkBitset(uint16Value, uint16Pos, uint16BitValue, %f, 16);
// uint32
checkBitset(uint32Value, uint32Pos, uint32BitValue + 1, %t, 32);
checkBitset(uint32Value, uint32Pos, uint32BitValue, %f, 32);

