// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Expand x to targetDims by repeating singleton axes
function y = broadcast_to(x, targetDims)
    dims = size(x);
    if length(dims) < length(targetDims) then
        dims($+1:length(targetDims)) = 1;
    end
    reps = int(targetDims ./ dims);
    y = repmat(x, reps);
endfunction

// N broadcast
a = rand(3, 4); b = rand(3, 4);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Broadcast on column (dim 2 = 1)
a = rand(3, 1); b = rand(3, 4);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Broadcast on line (dim 1 = 1)
a = rand(1, 5); b = rand(4, 5);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Leading ones
a = rand(1, 1, 3, 2); b = rand(4, 5, 3, 2);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Trailing ones
a = rand(4, 5, 1); b = rand(4, 5, 6);
tgt = max([size(a) 1; size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Multi-axis broadcast
a = rand(1, 3, 1, 2); b = rand(4, 1, 5, 2);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

a = rand(6, 1); b = rand(6, 7, 2);
tgt = max([size(a) 1; size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

a = rand(1, 7); b = rand(6, 7, 2);
tgt = max([size(a) 1; size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

a = rand(1, 1, 2); b = rand(6, 7, 2);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a + b, broadcast_to(a, tgt) + broadcast_to(b, tgt));

// Complex doubles
ca = [1+%i; -2+2*%i]; cb = [%i, 2-%i, -1+3*%i];
tgt = max([size(ca); size(cb)], "r");
assert_checkequal(ca + cb, broadcast_to(ca, tgt) + broadcast_to(cb, tgt));

// Integer types
i8 = int8([1; 2]); ui8 = uint8([10 20]);
tgt = max([size(i8); size(ui8)], "r");
assert_checkequal(i8 + ui8, broadcast_to(i8, tgt) + broadcast_to(ui8, tgt));

i16 = int16([5; 6]); ui16 = uint16([50 60 70]);
tgt = max([size(i16); size(ui16)], "r");
assert_checkequal(i16 + ui16, broadcast_to(i16, tgt) + broadcast_to(ui16, tgt));

i32 = int32([100; 200]); ui32 = uint32([1 2]);
tgt = max([size(i32); size(ui32)], "r");
assert_checkequal(i32 + ui32, broadcast_to(i32, tgt) + broadcast_to(ui32, tgt));

i64 = int64([5 10]); ui64 = uint64([100; 200]);
tgt = max([size(i64); size(ui64)], "r");
assert_checkequal(i64 + ui64, broadcast_to(i64, tgt) + broadcast_to(ui64, tgt));

// Strings
strCol = ["a"; "b"]; strRow = ["X" "Y" "Z"];
tgt = max([size(strCol); size(strRow)], "r");
assert_checkequal(strCol + strRow, broadcast_to(strCol, tgt) + broadcast_to(strRow, tgt));

// Polynomials
pcol = [%s; 1 + %s]; prow = [%s^2, 2 - %s];
tgt = max([size(pcol); size(prow)], "r");
assert_checkequal(pcol + prow, broadcast_to(pcol, tgt) + broadcast_to(prow, tgt));

// Complex polynomials
pcCol = [%s + %i; 2 - %i*%s]; pcRow = [%i*%s^2, 3 + 2*%i - %s];
tgt = max([size(pcCol); size(pcRow)], "r");
assert_checkequal(pcCol + pcRow, broadcast_to(pcCol, tgt) + broadcast_to(pcRow, tgt));

// With hypermatrix
a3 = rand(2, 2, 2);
a2 = rand(2, 2);

// --- addition ---
// double + double
assert_checktrue(a3 + a2 == a2 + a3);
// poly + double - double + poly
assert_checktrue(a3 * %s + a2 == a2 + a3 * %s);
assert_checktrue(a2 * %s + a3 == a3 + a2 * %s);

// --- dotdivide ---
//double ./ double
a3 = [1 1; 1 1];
a3(:,:,2) = [1 1;1 1];
a2 = [2 2; 2 2];
expected1(:,:,1) = 0.5 * ones(2,2);
expected1(:,:,2) = 0.5 * ones(2,2);
assert_checkequal(a3 ./ a2, expected1);

expected2(:,:,1) = 2 * ones(2,2);
expected2(:,:,2) = 2 * ones(2,2);
assert_checkequal(a2 ./ a3, expected2);

// poly ./ double
assert_checkequal((a3*%s) ./ a2, expected1 * %s);
assert_checkequal((a2*%s) ./ a3, expected2 * %s);

// --- dotmult ---
// double .* double
assert_checktrue(a3 .* a2 == a2 .* a3);
// poly .* double
assert_checkequal((a2 * %s) .* a3, a3 .* (a2 * %s));
assert_checkequal((a3 * %s) .* a2, a2 .* (a3 * %s));

// --- subtraction ---
// double - double
assert_checkequal(a3 - a2, -1 * ones(2,2,2));
assert_checkequal(a2 - a3, ones(2,2,2));
// poly - poly
assert_checkequal(a3*%s - a2*%s, -1*%s * ones(2,2,2));
assert_checkequal(a2*%s - a3*%s, ones(2,2,2)*%s);
// poly - double
assert_checkequal(a3*%s - a2, (%s - 2) * ones(2,2,2));
assert_checkequal(a2*%s - a3, (2*%s - 1) * ones(2,2,2));
// double - poly
assert_checkequal(a3 - a2 * %s, (1 - 2*%s) * ones(2,2,2));
assert_checkequal(a2 - a3*%s, (2 - 1 * %s) * ones(2,2,2));

// --- comparisons ---
// double < <= > >= double
a = rand(3, 1); b = rand(1, 4);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a < b, broadcast_to(a, tgt) < broadcast_to(b, tgt));
assert_checkequal(a <= b, broadcast_to(a, tgt) <= broadcast_to(b, tgt));
assert_checkequal(a > b, broadcast_to(a, tgt) > broadcast_to(b, tgt));
assert_checkequal(a >= b, broadcast_to(a, tgt) >= broadcast_to(b, tgt));

// with hypermatrix
a = rand(2, 1, 3); b = rand(1, 4, 3);
tgt = max([size(a); size(b)], "r");
assert_checkequal(a < b, broadcast_to(a, tgt) < broadcast_to(b, tgt));
assert_checkequal(a <= b, broadcast_to(a, tgt) <= broadcast_to(b, tgt));
assert_checkequal(a > b, broadcast_to(a, tgt) > broadcast_to(b, tgt));
assert_checkequal(a >= b, broadcast_to(a, tgt) >= broadcast_to(b, tgt));

A(:,:,2)=[1;1;1];
B(:,:,2)=[1,1,1];
tgt = max([size(A); size(B)], "r");
assert_checkequal(A < B, broadcast_to(A, tgt) < broadcast_to(B, tgt));
assert_checkequal(A <= B, broadcast_to(A, tgt) <= broadcast_to(B, tgt));
assert_checkequal(A > B, broadcast_to(A, tgt) > broadcast_to(B, tgt));
assert_checkequal(A >= B, broadcast_to(A, tgt) >= broadcast_to(B, tgt));
C = [1 1];
tgt = max([size(A); [size(C) 1]], "r");
assert_checkequal(A < C, broadcast_to(A, tgt) < broadcast_to(C, tgt));
assert_checkequal(A <= C, broadcast_to(A, tgt) <= broadcast_to(C, tgt));
assert_checkequal(A > C, broadcast_to(A, tgt) > broadcast_to(C, tgt));
assert_checkequal(A >= C, broadcast_to(A, tgt) >= broadcast_to(C, tgt));

// integer types
i8 = int8([1; 2]); j8 = int8([1 2 3]);
tgt = max([size(i8); size(j8)], "r");
assert_checkequal(i8 < j8, broadcast_to(i8, tgt) < broadcast_to(j8, tgt));
assert_checkequal(i8 <= j8, broadcast_to(i8, tgt) <= broadcast_to(j8, tgt));
assert_checkequal(i8 > j8, broadcast_to(i8, tgt) > broadcast_to(j8, tgt));
assert_checkequal(i8 >= j8, broadcast_to(i8, tgt) >= broadcast_to(j8, tgt));

ui32 = uint32([10; 20; 30]); vi32 = uint32([15 25]);
tgt = max([size(ui32); size(vi32)], "r");
assert_checkequal(ui32 < vi32, broadcast_to(ui32, tgt) < broadcast_to(vi32, tgt));
assert_checkequal(ui32 >= vi32, broadcast_to(ui32, tgt) >= broadcast_to(vi32, tgt));

// mixed integer/double (through %i_1_s overload)
i8 = int8([1; 2]); d = [0.5 1.5 2.5];
tgt = max([size(i8); size(d)], "r");
assert_checkequal(i8 < d, broadcast_to(i8, tgt) < broadcast_to(d, tgt));
assert_checkequal(d <= i8, broadcast_to(d, tgt) <= broadcast_to(i8, tgt));

// mixed integer widths (through %i_1_i overload)
i8 = int8([1; 2]); i16 = int16([0 1 2]);
tgt = max([size(i8); size(i16)], "r");
assert_checkequal(i8 < i16, broadcast_to(i8, tgt) < broadcast_to(i16, tgt));
assert_checkequal(i8 >= i16, broadcast_to(i8, tgt) >= broadcast_to(i16, tgt));

// per-column threshold use case
x = [1 5 3; 4 2 6]; th = [2 3 4];
assert_checkequal(x >= th, x >= repmat(th, 2, 1));

// interval membership use case
t = 1:5;
starts = [1; 3]; ends = [2; 5];
assert_checkequal(t >= starts & t <= ends, [%t %t %f %f %f; %f %f %t %t %t]);

// --- logical operators & | ---
// bool & | bool
b1 = [%t; %f]; b2 = [%t %f %t];
tgt = max([size(b1); size(b2)], "r");
assert_checkequal(b1 & b2, broadcast_to(b1, tgt) & broadcast_to(b2, tgt));
assert_checkequal(b1 | b2, broadcast_to(b1, tgt) | broadcast_to(b2, tgt));

// double & | bool mix
d = [1; 0]; b = [%t %f %t];
tgt = max([size(d); size(b)], "r");
assert_checkequal(d & b, broadcast_to(d, tgt) & broadcast_to(b, tgt));
assert_checkequal(b | d, broadcast_to(b, tgt) | broadcast_to(d, tgt));

// integers (bitwise), including mixed types
i8 = int8([1; 2]); j8 = int8([1 2 3]);
tgt = max([size(i8); size(j8)], "r");
assert_checkequal(i8 & j8, broadcast_to(i8, tgt) & broadcast_to(j8, tgt));
assert_checkequal(i8 | j8, broadcast_to(i8, tgt) | broadcast_to(j8, tgt));

u16 = uint16([1 2 3]); i8 = int8([1; 2]);
tgt = max([size(u16); size(i8)], "r");
assert_checkequal(u16 & i8, broadcast_to(u16, tgt) & broadcast_to(i8, tgt));
assert_checkequal(u16 | i8, broadcast_to(u16, tgt) | broadcast_to(i8, tgt));

// with hypermatrix
b1 = rand(2, 1, 3) > 0.5; b2 = rand(1, 4, 3) > 0.5;
tgt = max([size(b1); size(b2)], "r");
assert_checkequal(b1 & b2, broadcast_to(b1, tgt) & broadcast_to(b2, tgt));
assert_checkequal(b1 | b2, broadcast_to(b1, tgt) | broadcast_to(b2, tgt));

a3 = rand(2, 2, 2);
a2 = rand(2, 2);
tgt = max([size(a3); [size(a2) 1]], "r");
assert_checkequal(a3 & a2, broadcast_to(a3, tgt) & broadcast_to(a2, tgt));
assert_checkequal(a2 & a3, broadcast_to(a2, tgt) & broadcast_to(a3, tgt));
assert_checkequal(a3 | a2, broadcast_to(a3, tgt) | broadcast_to(a2, tgt));
assert_checkequal(a2 | a3, broadcast_to(a2, tgt) | broadcast_to(a3, tgt));

assert_checkequal(int8(a3) & int8(a2), broadcast_to(int8(a3), tgt) & broadcast_to(int8(a2), tgt));
assert_checkequal(int8(a2) & int8(a3), broadcast_to(int8(a2), tgt) & broadcast_to(int8(a3), tgt));
assert_checkequal(int8(a3) | int8(a2), broadcast_to(int8(a3), tgt) | broadcast_to(int8(a2), tgt));
assert_checkequal(int8(a2) | int8(a3), broadcast_to(int8(a2), tgt) | broadcast_to(int8(a3), tgt));

// Incompatibility (raise an error)
msg = sprintf(_("Operator %ls: Wrong dimensions for operation [%ls] %ls [%ls], same dimensions expected.\n"), "+", "2x3", "+", "4x1");
assert_checkerror("rand(2,3) + rand(4,1)", msg);

msg = sprintf(_("Operator %ls: Wrong dimensions for operation [%ls] %ls [%ls], same dimensions expected.\n"), "&", "2x3", "&", "4x1");
assert_checkerror("rand(2,3) & rand(4,1)", msg);

msg = sprintf(_("Operator %ls: Wrong dimensions for operation [%ls] %ls [%ls], same dimensions expected.\n"), "|", "2x3", "|", "4x1");
assert_checkerror("rand(2,3) | rand(4,1)", msg);

msg = sprintf(_("Operator %ls: Wrong dimensions for operation [%ls] %ls [%ls], same dimensions expected.\n"), "<", "2x3", "<", "4x1");
assert_checkerror("rand(2,3) < rand(4,1)", msg);

msg = sprintf(_("Operator %ls: Wrong dimensions for operation [%ls] %ls [%ls], same dimensions expected.\n"), "<=", "2x3", "<=", "4x1");
assert_checkerror("rand(2,3) <= rand(4,1)", msg);
