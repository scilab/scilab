// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// =============================================
//  unitary tests for fftshift() and ifftshift()
// =============================================

vec = 1:60;
res = ifftshift(vec);
assert_checkequal(res(1:30), 31:60);
assert_checkequal(res(31:60), 1:30);

mat = matrix(vec, [10 6]);
refRES = (1:5)';
refRES = [refRES refRES+10 refRES+20];
res = ifftshift(mat);
assert_checkequal(res(1:5, 1:3), refRES+35);
assert_checkequal(res(1:5, 4:6), refRES+5);
assert_checkequal(res(6:10, 1:3), refRES+30);
assert_checkequal(res(6:10, 4:6), refRES);


// Even input sizes
// ================
// []
assert_checkequal(fftshift([]),    []);
assert_checkequal(fftshift([],1),  []);
assert_checkequal(fftshift([],"r"),[]);
assert_checkequal(fftshift([],2),  []);
assert_checkequal(fftshift([],"c"),[]);
assert_checkequal(ifftshift([]),    []);
assert_checkequal(ifftshift([],1),  []);
assert_checkequal(ifftshift([],"r"),[]);
assert_checkequal(ifftshift([],2),  []);
assert_checkequal(ifftshift([],"c"),[]);

// Vector
v = [1 2 3 4];
r = [3 4 1 2];
assert_checkequal(fftshift(v),  r);
assert_checkequal(fftshift(v,1), v);
assert_checkequal(fftshift(v,"r"), v);
assert_checkequal(fftshift(v,2), r);
assert_checkequal(fftshift(v,"c"), r);

assert_checkequal(ifftshift(v),  r);
assert_checkequal(ifftshift(v,1), v);
assert_checkequal(ifftshift(v,"r"), v);
assert_checkequal(ifftshift(v,2), r);
assert_checkequal(ifftshift(v,"c"), r);

// Matrix
m = [
  1   2     3   4
  5   6     7   8

  9  10    11  12
  13 14    15  16
];
r = [
  11 12    9 10
  15 16   13 14

   3  4    1  2
   7  8    5  6
];
rr = [
   9 10   11 12
  13 14   15 16

   1  2    3  4
   5  6    7  8
];
rc = [
   3  4    1  2
   7  8    5  6

  11 12    9 10
  15 16   13 14
];
assert_checkequal(fftshift(m),    r);
assert_checkequal(fftshift(m,1),  rr);
assert_checkequal(fftshift(m,"r"),rr);
assert_checkequal(fftshift(m,2),  rc);
assert_checkequal(fftshift(m,"c"),rc);

assert_checkequal(ifftshift(m),    r);
assert_checkequal(ifftshift(m,1),  rr);
assert_checkequal(ifftshift(m,"r"),rr);
assert_checkequal(ifftshift(m,2),  rc);
assert_checkequal(ifftshift(m,"c"),rc);

// Hypermatrix
h = matrix(1:8,2,2,2);
//(:,:,1)
//   1   3
//   2   4
//(:,:,2)
//   5   7
//   6   8
assert_checkequal(fftshift(h)   ,  cat(3,[8 6;7 5], [4 2;3 1]));
assert_checkequal(ifftshift(h)  ,  cat(3,[8 6;7 5], [4 2;3 1]));
assert_checkequal(fftshift(h,1) ,  cat(3,[2 4;1 3], [6 8;5 7]));
assert_checkequal(ifftshift(h,1),  cat(3,[2 4;1 3], [6 8;5 7]));
assert_checkequal(fftshift(h,2) ,  cat(3,[3 1;4 2], [7 5;8 6]));
assert_checkequal(ifftshift(h,2),  cat(3,[3 1;4 2], [7 5;8 6]));
assert_checkequal(fftshift(h,3) ,  cat(3,[5 7;6 8], [1 3;2 4]));
assert_checkequal(ifftshift(h,3),  cat(3,[5 7;6 8], [1 3;2 4]));


// Odd input sizes
// ===============
// Vector
v = [1 2 3 4 5];
r = [4 5 1 2 3];
assert_checkequal(fftshift(v),  r);
assert_checkequal(fftshift(v,1), v);
assert_checkequal(fftshift(v,"r"), v);
assert_checkequal(fftshift(v,2), r);
assert_checkequal(fftshift(v,"c"), r);

assert_checkequal(ifftshift(fftshift(v)), v);
assert_checkequal(ifftshift(fftshift(v,1),1), v);
assert_checkequal(ifftshift(fftshift(v,2),2), v);
assert_checkequal(ifftshift(fftshift(v,"r"),"r"), v);
assert_checkequal(ifftshift(fftshift(v,"c"),"c"), v);

// Matrix
m = [
  1  2  3    4  5
  6  7  8    9 10

 11 12 13   14 15
];
r = [
 14 15   11 12 13

  4  5    1  2  3
  9 10    6  7  8
];
rr = [
 11 12 13   14 15

  1  2  3    4  5
  6  7  8    9 10
];
rc = [
  4  5   1  2  3
  9 10   6  7  8

  14 15  11 12 13
];
assert_checkequal(fftshift(m),    r);
assert_checkequal(fftshift(m,1),  rr);
assert_checkequal(fftshift(m,"r"),rr);
assert_checkequal(fftshift(m,2),  rc);
assert_checkequal(fftshift(m,"c"),rc);

m = [
  1  2    3  4  5

  6  7    8  9 10
 11 12   13 14 15
];
r = [
  8  9 10    6  7
 13 14 15   11 12

  3  4  5    1  2
];
rr = [
  6  7    8  9 10
 11 12   13 14 15

  1  2    3  4  5
];
rc = [
  3  4  5   1  2

  8  9 10   6  7
 13 14 15  11 12
];
assert_checkequal(ifftshift(m),    r);
assert_checkequal(ifftshift(m,1),  rr);
assert_checkequal(ifftshift(m,"r"),rr);
assert_checkequal(ifftshift(m,2),  rc);
assert_checkequal(ifftshift(m,"c"),rc);

// Hypermatrix
// -----------
h = cat(3, ..
[
  1  4    7
  2  5    8

  3  6    9
], [
  10  13    16
  11  14    17

  12  15    18
], ..
[
  19  22    25
  20  23    26

  21  24    27
]);

// along = "all"
res = cat(3, ..
[
  27    21  24

  25    19  22
  26    20  23
], [
  9    3  6

  7    1  4
  8    2  5
], [
  18    12  15

  16    10  13
  17    11  14
]);
assert_checkequal(fftshift(h), res);
assert_checkequal(fftshift(ifftshift(h)), h);
assert_checkequal(ifftshift(fftshift(h)), h);

// along = 1 | "r"
res = cat(3, ..
[
  3  6  9
  1  4  7
  2  5  8
], [
  12  15  18
  10  13  16
  11  14  17
], [
  21  24  27
  19  22  25
  20  23  26
]);
assert_checkequal(fftshift(h,1), res);
assert_checkequal(ifftshift(fftshift(h,1),1), h);
assert_checkequal(fftshift(ifftshift(h,1),1), h);

// along = 2 | "c"
res = cat(3, ..
[
  7  1  4
  8  2  5
  9  3  6
], [
  16  10  13
  17  11  14
  18  12  15
], [
  25  19  22
  26  20  23
  27  21  24
]);
assert_checkequal(fftshift(h,2), res);
assert_checkequal(ifftshift(fftshift(h,2),2), h);
assert_checkequal(fftshift(ifftshift(h,2),2), h);

// along = 3
res = cat(3, ..
[
  19  22  25
  20  23  26
  21  24  27
], [
  1  4  7
  2  5  8
  3  6  9
], [
  10  13  16
  11  14  17
  12  15  18
]);
assert_checkequal(fftshift(h,3), res);
assert_checkequal(ifftshift(fftshift(h,3),3), h);
assert_checkequal(fftshift(ifftshift(h,3),3), h);
