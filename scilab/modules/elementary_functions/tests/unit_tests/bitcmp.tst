// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit test for bitcmp function
// =============================================================================

function r = cint64(valstr)
    // Returns the uint64 or int64 value matching its given literal representation
    s = strsplit(valstr)($:-1:1);
    neg = s($)=="-"
    if neg, s($) = [], end
    s = evstr(s);
    r = 0
    for i = 1:size(s,"*")
        r = r + uint64(10).^(i-1)*s(i)
    end
    if neg, r = -int64(r), end
endfunction

// =====================
// With encoded integers
// =====================
// Tests for one input argument
// ----------------------------
assert_checkequal(bitcmp(int8(99)),  int8(-100));
assert_checkequal(bitcmp(int16(99)), int16(-100));
assert_checkequal(bitcmp(int32(99)), int32(-100));
assert_checkequal(bitcmp(int64(99)), int64(-100));

assert_checkequal(bitcmp(uint8(99)),  uint8(156));
assert_checkequal(bitcmp(uint16(99)), uint16(65436));
assert_checkequal(bitcmp(uint32(99)), uint32(4294967196));
assert_checkequal(bitcmp(uint64(99)), cint64("18446744073709551516"));
assert_checkequal(bitcmp(cint64("18446744073709551516")), uint64(99));

assert_checkequal(bitcmp(int8([13,99])),  int8([-14 -100]));
assert_checkequal(bitcmp(int16([13,99])), int16([-14 -100]));
assert_checkequal(bitcmp(int32([13,99])), int32([-14 -100]));
assert_checkequal(bitcmp(int64([13,99])), int64([-14 -100]));

assert_checkequal(bitcmp(uint8([13,99])),  uint8([242,156]));
assert_checkequal(bitcmp(uint16([13,99])), uint16([65522,65436]));
assert_checkequal(bitcmp(uint32([13,99])), uint32([4294967282,4294967196]));
assert_checkequal(bitcmp(uint64([13,99])), [cint64("18446744073709551602") cint64("18446744073709551516")]);
assert_checkequal(bitcmp([cint64("18446744073709551602") cint64("18446744073709551516")]), uint64([13,99]));
m = uint64(%inf);
r = bitcmp([uint64(0:2), m-2, m-1, m]);
assert_checkequal(r, [cint64("18446744073709551613")+[2 1 0] uint64([2 1 0])]);

// Tests for two input arguments
// -----------------------------
// scalar bitnum
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 8), int8([70 33 0 -1 -34 -71]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 7), int8([70 33 0 127 94 57]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 6), int8([6 33 0 63 30 57]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 5), int8([6 1 0 31 30 25]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 4), int8([6 1 0 15 14 9]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 3), int8([6 1 0 7 6 1]));
assert_checkequal(bitcmp(int8([-71 -34 -1 0 33 70]), 2), int8([2 1 0 3 2 1]));

assert_checkequal(bitcmp(uint8(99),8), uint8(156));
assert_checkequal(bitcmp(uint16(99),16), uint16(65436));
assert_checkequal(bitcmp(uint32(99),32), uint32(4294967196));

assert_checkequal(bitcmp(uint8([13,99]),8),  uint8([242,156]));
assert_checkequal(bitcmp(uint16([13,99]),16), uint16([65522,65436]));
assert_checkequal(bitcmp(uint32([13,99]),32), uint32([4294967282,4294967196]));
ui64 = cint64("1844674407370955161");
b = bitget(ui64,1:64);
for i = 2:63
    assert_checkequal(bitcmp(ui64, i), bitset(uint64(0), 1:i, 1-b(1:i)));
end
// bitnum array
[in, out] = ([0 0 0 0 0], [7 15 31 63 127]);
assert_checkequal(bitcmp(uint8(in), 3:7), uint8(out));
assert_checkequal(bitcmp(uint16(in), 3:7), uint16(out));
assert_checkequal(bitcmp(uint32(in), 3:7), uint32(out));
assert_checkequal(bitcmp(uint64(in), 3:7), uint64(out));
assert_checkequal(bitcmp(in, 3:7), out);

// =====================
// With decimal integers
// =====================
assert_checkequal(bitcmp(99,8), 156);
assert_checkequal(bitcmp(99,16), 65436);
assert_checkequal(bitcmp(99,32), 4294967196);

assert_checkequal(bitcmp([13,99],8),  [242,156]);
assert_checkequal(bitcmp([13,99],16), [65522,65436]);
assert_checkequal(bitcmp([13,99],32), [4294967282,4294967196]);

assert_checkequal(bitcmp(0:10, 8), double(bitcmp(uint8(0:10))));
assert_checkequal(bitcmp(0:10, 16), double(bitcmp(uint16(0:10))));
assert_checkequal(bitcmp(0:10, 32), double(bitcmp(uint32(0:10))));
assert_checkequal(bitcmp(0:10, 40)-2^40, -1:-1:-11);
assert_checkequal(bitcmp(0:10, 52)-2^52, -1:-1:-11);
assert_checkequal(bitcmp(2^52 + (-11:-1),52), 10:-1:0);
assert_checkequal(bitcmp(2^40 + (-11:-1), 40), 10:-1:0);

// with bitnum > 53
// ----------------
b = bitget(1e100,281:333); // log2(1e100) == 332.ddd
bcmp332 = bitset(0,281:333,1-b); // 7.4980057982640933D+99
assert_checkalmostequal(bitcmp(1e100,333), bcmp332, %eps);
assert_checkalmostequal(bitcmp(1e100), bcmp332, %eps);
assert_checkalmostequal(bitcmp(bcmp332,333), 1e100, %eps);
r = bitcmp(0,333);
assert_checkalmostequal(bitcmp(r,333)/2^333, 0, 0, %eps);

V = [1e100 2e150 1e200 3e250 1e300];
// one by one:
for v = V
    n = int(log2(v)) + 1;
    bits = bitget(v, n:-1:n-51);
    cbits = bitget(bitcmp(v,n), n:-1:n-51);
    assert_checktrue(and((bits+cbits)==1));
end
// in a vector:
n = int(log2(V)) + 1;
    // with distinct implicit bitnums
r = bitcmp(V);
r = bitcmp(r, n);
assert_checkalmostequal(r, V, %eps);


// ==============
// Error messages
// ==============
errmsg = msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "bitcmp", 1, 2);
assert_checkerror("bitcmp()", errmsg);       // no input argument
errmsg = _("Wrong number of input arguments.")
assert_checkerror("bitcmp(1,2,3)", errmsg);  // too many argins

// argin#1
errmsg = msprintf(_("%s: Argument #%d: Decimal or encoded integers expected.\n"), "bitcmp", 1);
assert_checkerror("bitcmp(""s"")", errmsg);
assert_checkerror("bitcmp(%i)", errmsg);
errmsg = msprintf(_("%s: Argument #%d: Positive decimal integers expected.\n"), "bitcmp", 1);
assert_checkerror("bitcmp(1.2)", errmsg);
assert_checkerror("bitcmp(-2)", errmsg);

// argin#2
errmsg = msprintf(_("%s: Argument #%d: Wrong size.\n"), "bitcmp", 2);
assert_checkerror("bitcmp(2, [1 2])", errmsg);
errmsg = msprintf(_("%s: Argument #%d: Real or encoded integer expected.\n"), "bitcmp", 2);
assert_checkerror("bitcmp(2, 4.5)", errmsg);
assert_checkerror("bitcmp(2, 3+%i)", errmsg);
errmsg = msprintf(_("%s: Argument #%d: Must be in the interval [%d, %d].\n"),"bitcmp", 2, 1, 8);
assert_checkerror("bitcmp(uint8(99), 9)", errmsg);
assert_checkerror("bitcmp(uint8(99), 16)", errmsg);
