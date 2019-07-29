// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2013, 2014 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// Along dimension 1:
x = [1 2 3 4; 5 6 7 8];
dim = 1;
y = flipdim(x, dim);
assert_checkequal([5 6 7 8; 1 2 3 4], y);

// Along dimension 2:
dim = 2;
y = flipdim(x, dim);
assert_checkequal([4 3 2 1; 8 7 6 5], y);

// Along dimension 3:
x = matrix(1:24, [3 2 4]);
dim = 3;
ref = matrix( [19;20;21;22;23;24;13;14;15;16;17;18;7;8;9;10;11;12;1;2;3;4;5;6], [3 2 4]);
y = flipdim(x, dim);
assert_checkequal(y, ref);

// Complexes:
y = flipdim(x*%i, dim);
assert_checkequal(y, ref*%i);

// Integer-encoded numbers:
y = flipdim(int16(x), dim);
assert_checkequal(y, int16(ref));

// Booleans:
ref_bool = matrix([%T;%T;%T;%T;%T;%T;%T;%T;%T;%T;%T;%T;%F;%F;%F;%T;%T;%F;%F;%F;%F;%F;%F;%F], [3 2 4]);
y = flipdim(x < 15, 2);
assert_checkequal(y, ref_bool);

// Texts:
x_txt = matrix(strsplit("a":"x", 1:23), 4, 6);
x_txt = x_txt + x_txt;
ref_txt = ["uu" "qq" "mm" "ii" "ee" "aa"; "vv" "rr" "nn" "jj" "ff" "bb";
"ww" "ss" "oo" "kk" "gg" "cc";
"xx" "tt" "pp" "ll" "hh" "dd" ];
y = flipdim(x_txt, 2);
assert_checkequal(y, ref_txt);

// Polynomials:
x_pol = inv_coeff([x(:,:,1) x(:,:,2) x(:,:,3) x(:,:,4) [0;1;2]], 2);
X = poly(0, "x");
ref_pol = [3+12*X+21*X^2 6+15*X+24*X^2 9+18*X+2*X^2;
2+11*X+20*X^2 5+14*X+23*X^2 8+17*X+X^2;
1+10*X+19*X^2 4+13*X+22*X^2 7+16*X ];
y = flipdim(x_pol, 1);
assert_checkequal(y, ref_pol);

// Rationals:
r = (x_pol) ./ X;
ref_r = ref_pol ./ X;
y = flipdim(r, 1);
assert_checkequal(y, ref_r);

// Hypermatrices:
x = matrix(1:48, [3 2 4 2]);
dim = 3;
ref = matrix([19;20;21;22;23;24;13;14;15;16;17;18;7;8;9;10;11;12;1;2;3;4;5;6;43;44;45;46;47;48;37;38;39;40;41;42;31;32;33;34;35;36;25;26;27;28;29;30], [3 2 4 2]);
y = flipdim(x, dim);
assert_checkequal(y, ref);

// ==================
// BlockSize argument
// ==================
x = [0 1 2 3 4 5 6 7 8 9 10 11];
x = [x ; x];

ref1 = [11 10 9 8 7 6 5 4 3 2 1 0];
ref1 = [ref1 ; ref1];
y = flipdim(x, 2, 1); // Present action.
assert_checkequal(y, ref1);

ref2 = [10 11   8 9   6 7   4 5   2 3   0 1];
ref2 = [ref2 ; ref2];
y = flipdim(x, 2, 2); // Block size = 2.
assert_checkequal(y, ref2);

ref3 = [9 10 11   6 7 8   3 4 5   0 1 2];
ref3 = [ref3 ; ref3];
y = flipdim(x, 2, 3);
assert_checkequal(y, ref3);

ref4 = [8 9 10 11   4 5 6 7   0 1 2 3];
ref4 = [ref4 ; ref4];
y = flipdim(x, 2, 4);
assert_checkequal(y, ref4);

ref5 = [6 7 8 9 10 11   0 1 2 3 4 5];
ref5 = [ref5 ; ref5];
y = flipdim(x, 2, 6);
assert_checkequal(y, ref5);

// Error checks
// ------------
refMsg = msprintf(_("%s: Wrong value for input argument #%d: A divisor of the selected dimension size expected.\n"), "flipdim", 3);
assert_checkerror("y = flipdim(x, 2, 5)", refMsg); // size(x) = [2 12] and 5 does not divide 12.

// With an hypermatrix
// -------------------
h = matrix(1:16,2,4,2);
// h  =
//(:,:,1)
//   1.   3.   5.   7.
//   2.   4.   6.   8.
//(:,:,2)
//   9.    11.   13.   15.
//   10.   12.   14.   16.
ref = cat(3, [5 7 1 3; 6 8 2 4], [13 15 9 11; 14 16 10 12]);
assert_checkequal(flipdim(h,2,2), ref);

h = permute(h,[2 1 3]);
// h  =
//(:,:,1)
//   1.   2.
//   3.   4.
//   5.   6.
//   7.   8.
//(:,:,2)
//   9.    10.
//   11.   12.
//   13.   14.
//   15.   16.
ref = cat(3, [5 6; 7 8; 1 2; 3 4], [13 14; 15 16; 9 10; 11 12]);
assert_checkequal(flipdim(h,1,2), ref);

h = permute(h,[3 2 1]);
// h  =
//(:,:,1)
//   1.   2.
//   9.   10.
//(:,:,2)
//   3.    4.
//   11.   12.
//(:,:,3)
//   5.    6.
//   13.   14.
//(:,:,4)
//   7.    8.
//   15.   16.
ref = cat(3,[5 6 ; 13 14], [7 8 ; 15 16], [1 2 ; 9 10], [3 4 ; 11 12]);
assert_checkequal(flipdim(h,3,2), ref);
