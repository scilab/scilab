// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Unit test of vectorfind() -->

m = [
  1  0  0  0  1  0  1  0  1  0  0  1  1  1  1
  0  0  1  0  0  1  0  1  0  0  0  0  0  0  0
  0  1  1  1  0  0  1  1  1  0  1  1  0  0  1
  0  0  1  0  1  0  1  0  1  0  0  0  0  0  1
  ];
H = [1 0 0 1 2 0 0 0 1 0 2 1 2 2 1 0 1 2
     2 2 1 0 0 2 1 0 2 0 1 2 1 0 0 1 0 0];
H = matrix(H, [2 3 3 2])
// --> H = matrix(H, [2 3 3 2])
//  H  =
// (:,:,1,1)
//    1   0   0
//    2   2   1
// (:,:,2,1)
//    1   2   0
//    0   0   2
// (:,:,3,1)
//    0   0   1
//    1   0   2
//
// (:,:,1,2)
//    0   2   1
//    0   1   2
// (:,:,2,2)
//    2   2   1
//    1   0   0
// (:,:,3,2)
//    0   1   2
//    1   0   0

// ==================
// CHECKING ARGUMENTS
// ==================
// Number of arguments
assert_checkfalse(execstr("vectorfind()","errcatch")==0);
assert_checkfalse(execstr("vectorfind(m,[1 0 1 0],""c"",2,"""",%t)","errcatch")==0);
assert_checktrue (execstr("a=vectorfind(m,[1 0 1 0],""c"")","errcatch")==0);
assert_checktrue (execstr("[a,b]=vectorfind(m,[1 0 1 0],""c"")","errcatch")==0);
assert_checkfalse(execstr("[a,b,c]=vectorfind(m,[1 0 1 0],""c"")","errcatch")==0);

// Needle's orientation does not matter:
assert_checkequal(vectorfind(m, [0 0 1 0], "c"), [2 4 11]);
assert_checkequal(vectorfind(m, [0 0 1 0]', "c"), [2 4 11]);
assert_checkequal(vectorfind(m, cat(3, 0,0,1,0), "c"), [2 4 11]);
assert_checkequal(vectorfind(H, [0 1], "c"), [3 7 16]);
assert_checkequal(vectorfind(H, [0 1]', "c"), [3 7 16]);
assert_checkequal(vectorfind(H, cat(3, 0,1), "c"), [3 7 16]);

// Empty needle gets empty result:
assert_checkequal(vectorfind(m, []), []);
assert_checkequal(vectorfind(m, [], "c"), []);
assert_checkequal(vectorfind(H, []), []);
assert_checkequal(vectorfind(H, [], "c"), []);
assert_checkequal(vectorfind(H, [], 3), []);
assert_checkequal(vectorfind(H, [], 4), []);

// Too long needles get empty results:
assert_checkequal(vectorfind(m, 1:5, "c"), []);   // too tall v
assert_checkequal(vectorfind(m, 1:16, "r"), []);  // too long v
assert_checkequal(vectorfind(H, [0 1 0 2], "r"), []);
assert_checkequal(vectorfind(H, [0 1 0], "c"), []); // size = 2
assert_checkequal(vectorfind(H, [0 1 2 3], 3), []); // size = 3
assert_checkequal(vectorfind(H, [0 1 2], 4), []);   // size = 2

// Without joker, the matching array is empty 
[i,ma] = vectorfind(m, [0 0 1 0], "c");
assert_checkequal(ma, []);
[i,ma] = vectorfind(m, [0 0 1 0], "c",,"headIJK");
assert_checkequal(ma, []);

// ================
// CHECKING RESULTS
// ================
// ====================
// WITH DECIMAL NUMBERS
// ====================
m = [
//1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  1  0  0  0  1  0  1  0  1  0  0  1  1  1  1
  0  0  1  0  0  1  0  1  0  0  0  0  0  0  0
  0  1  1  1  0  0  1  1  1  0  1  1  0  0  1
  0  0  1  0  1  0  1  0  1  0  0  0  0  0  1
  ];
assert_checkequal(vectorfind(m, [1 1 1 1 0 0 1 1 1 0 1 1 0 0 1]), []);
assert_checkequal(vectorfind(m, [0 1 1 1 0 0 1 1 1 0 1 1 0 0 1]), 3);
assert_checkequal(vectorfind(m, [1 0 0 0], "c"), [1 13 14]);

// With %inf
m(2,2) = %inf;
assert_checkequal(vectorfind(m, [0 %inf 1 0]), 2);

// With %nan in the haystack
m(2,2) = %nan;
assert_checkequal(vectorfind(m, [0 1 1 0], "c"), 8);
assert_checkequal(vectorfind(m, [%nan 1]), 6);    // Matches as any other value

// With short v
assert_checkequal(vectorfind(m,[1 0 1],"c"), [25 33 45 57]);
assert_checkequal(vectorfind(m,[1 1 0 0],"r"), [11 43]);

// Option indType
assert_checkequal(vectorfind(m, [1 0 0 0], "c"), [1 13 14]);
assert_checkequal(vectorfind(m, [1 0 0 0], "c",,"headN"), [1 49 53]);
assert_checkequal(vectorfind(m, [1 0 0 0], "c",,"headIJK"), [1 1 ; 1 13 ; 1 14]);
assert_checkequal(vectorfind(m, [0 1 1 1 0 0 1 1 1 0 1 1 0 0 1],"r",,"headN"), 3);
assert_checkequal(vectorfind(m, [0 1 1 1 0 0 1 1 1 0 1 1 0 0 1],"r",,"headIJK"), [3 1]);

// With jokers in the needle
[ind, ma] = vectorfind(m, [1 -1 -1 0],"c",-1);
assert_checkequal(ind, [1 12 13 14]);
assert_checkequal(ma, [1 0 0 0; 1 0 1 0; 1 0 0 0; 1 0 0 0]);
// With %nan jokers in the needle
assert_checkequal(vectorfind(m, [1 %nan %nan 0],"c",%nan), [1 12 13 14]);

// With %nan in the haystack, ignored by %nan in the needle
[ind, ma] = vectorfind(m, [0 %nan 1 0],"c",%nan);
assert_checkequal(ind, [2 4 8 11]);
assert_checkequal(ma, [0 %nan 1 0; 0 0 1 0; 0 1 1 0; 0 0 1 0]);

// With %nan jokers in the needle
assert_checkequal(vectorfind(m,[1 1 %nan 0],"r",%nan), [7 11 27 43]);
assert_checkequal(vectorfind(m,[0 %nan %nan 0 0 1],"r",%nan), [2 39 40]);
assert_checkequal(vectorfind(m,[0 %nan 1],"c",%nan), [5 9 13 18 26 29 34 41 58]);

// Hypermatrix
// ===========
// Option indType
    //              dir = "r" = 1
assert_checkequal(vectorfind(H, [1 0 0]), [1 10 12]);
assert_checkequal(vectorfind(H, [1 0 0], "r",,"headN"), [1 26 32]);
res =  [1  1  1  1
        2  1  2  2
        2  1  3  2];
assert_checkequal(vectorfind(H, [1 0 0], "r",,"headIJK"), res);
    //              dir = "c" = 2
assert_checkequal(vectorfind(H, [0 1], "c"), [3 7 16]);
assert_checkequal(vectorfind(H, [0 1], "c",,"headN"), [5 13 31]);
res =  [1  3  1  1
        1  1  3  1
        1  1  3  2];
assert_checkequal(vectorfind(H, [0 1], "c",,"headIJK"), res);
    //              dir = 3
assert_checkequal(vectorfind(H, [0 2 0], 3), [3 7]);
assert_checkequal(vectorfind(H, [0 2 0], 3,,"headN"), [3 19]);
res =  [1  2  1  1
        1  1  1  2];
assert_checkequal(vectorfind(H, [0 2 0], 3,,"headIJK"), res);
    //              dir = 4
assert_checkequal(vectorfind(H, [0 1], 4), [5 8 11 15]);
assert_checkequal(vectorfind(H, [0 1], 4,,"headN"), [5 8 11 15]);
res =  [1  3  1  1
        2  1  2  1
        1  3  2  1
        1  2  3  1];
assert_checkequal(vectorfind(H, [0 1], 4,,"headIJK"), res);

// With short v
// ------------
//  H  =
// (:,:,1,1)
//    1   0   0
//    2   2   1
// (:,:,2,1)
//    1   2   0
//    0   0   2
// (:,:,3,1)
//    0   0   1
//    1   0   2
//
// (:,:,1,2)
//    0   2   1
//    0   1   2
// (:,:,2,2)
//    2   2   1
//    1   0   0
// (:,:,3,2)
//    0   1   2
//    1   0   0
assert_checkequal(vectorfind(H, [0 1], "r"), [15 20 31]);
assert_checkequal(vectorfind(H, [0 1], 1  ), [15 20 31]);
res = [1  2  3  1
       2  1  1  2
       1  1  3  2];
assert_checkequal(vectorfind(H, [0 1], "r",,"headIJK"), res);
assert_checkequal(vectorfind(H, 2, "c"), [2 4 9 12 18 21 24 25 27 35]);

assert_checkequal(vectorfind(H, [0 0], 3), [5 10 28 30]);
res = [
  1  3  1  1
  2  2  2  1
  2  2  2  2
  2  3  2  2];
assert_checkequal(vectorfind(H, [0 0], 3,, "headIJK"), res);
H2 = matrix(H,1,3,3,4);
assert_checkequal(vectorfind(H2, [0 1], 4), [5 8 13 20]);
res = [
  1  2  2  1
  1  2  3  1
  1  1  2  2
  1  2  1  3];
assert_checkequal(vectorfind(H2, [0 1], 4,, "headIJK"), res);

// Additional tests with an hypermatrix haystack
// ---------------------------------------------
m = matrix(m,4,5,3);
m(2,2) = 0;
//(:,:,1)
//   1   0   0   0   1
//   0   0   1   0   0
//   0   1   1   1   0
//   0   0   1   0   1
//(:,:,2)
//   0   1   0   1   0
//   1   0   1   0   0
//   0   1   1   1   0
//   0   1   0   1   0
//(:,:,3)
//   0   1   1   1   1
//   0   0   0   0   0
//   1   1   0   0   1
//   0   0   0   0   1
assert_checkequal(vectorfind(m, [1 0 1 1], "c"), [7 9 15]); // linearized columns index
assert_checkequal(vectorfind(m, [0 1 0 1 0], "r"), [5 8]);  // linearized rows index

// Short needle
// ------------
    // default indType = headN
assert_checkequal(vectorfind(m, [1 1 0], "c"), 30);
assert_checkequal(vectorfind(m, [1 1 1], "r"), [7 27 45 49]);
assert_checkequal(vectorfind(m, [1 1], 3), [7 10 11 15 25 27 33]);
    // indType = headIJK
assert_checkequal(vectorfind(m, [1 1 0], "c",,"headIJK"), [2 3 2]);
res = [3  2  1
       3  2  2
       1  2  3
       1  3  3];
assert_checkequal(vectorfind(m, [1 1 1], "r",,"headIJK"), res);
res = [
  3  2  1
  2  3  1
  3  3  1
  3  4  1
  1  2  2
  3  2  2
  1  4  2];
assert_checkequal(vectorfind(m, [1 1], 3,,"headIJK"), res);

// With jokers in the needle:
// -------------------------
//  Columns starting and ending with 1:
[ind, ma] = vectorfind(m, [1 %nan %nan 1], "c", %nan);
assert_checkequal(ind, [5 7 9 15]);
assert_checkequal(ma, [1 0 0 1; 1 0 1 1; 1 0 1 1; 1 0 1 1]);
//  Rows starting and ending with 1:
assert_checkequal(vectorfind(m, [1 %nan %nan %nan 1], "r", %nan), [1 11]);
//  Stack starting and ending with 1:
[ind, ma] = vectorfind(m, [1 %nan 1], 3, %nan);
assert_checkequal(ind, [7 17 20]);
assert_checkequal(ma, [1 1 1; 1 0 1; 1 0 1]);

// With jokers in short needle:
// ---------------------------
[ind, ma] = vectorfind(m, [1 %nan 1], 2, %nan);
assert_checkequal(ind, [10 25 33 45 57]);
assert_checkequal(ma, [1 1 1; 1 0 1; 1 0 1; 1 0 1; 1 0 1]);
res = [1  2  1
       3  2  1
       1  4  1];
assert_checkequal(vectorfind(m, [%nan 1 1], 3, %nan, "headIJK"), res);

// ===================
// WITH NUMERIC SPARSE
// ===================
m = [
  0  0  1  1  1
  1  0  0  2  0
  2  0  2  2  1];
m = sparse(m);
assert_checkequal(vectorfind(m,[0 0 0],"c"), 2);
assert_checkequal(vectorfind(m,[1 0 0 2 0],"r"), 2);
// Joker
[ind, ma] = vectorfind(m,[1 %nan 2],"c", %nan);
assert_checkequal(ind, [3 4]);
assert_checkequal(ma, sparse([1 0 2 ; 1 2 2]));
// Short needle
assert_checkequal(vectorfind(m,[0 0],"r"), [1 5]);
// Short needle with joker
[ind, ma] = vectorfind(m,[0 %nan 2],"r", %nan);
assert_checkequal(ind, [5 6]);
assert_checkequal(ma, sparse([0 0 2 ; 0 2 2]));


// ==========
// WITH TEXTS
// ==========
m  = [
// 1    2    3    4    5    6    7    8    9   10   11   12
  "U"  "C"  "G"  "A"  "A"  "A"  "U"  "U"  "A"  "G"  "A"  "G"
  "A"  "A"  "A"  "A"  "C"  "C"  "U"  "U"  "C"  "G"  "G"  "G"
  "A"  "G"  "A"  "C"  "G"  "C"  "C"  "C"  "G"  "C"  "A"  "G"
  "C"  "U"  "G"  "G"  "G"  "A"  "A"  "G"  "C"  "C"  "C"  "C"
  "C"  "G"  "G"  "A"  "A"  "G"  "U"  "C"  "A"  "U"  "G"  "C"
  ];
// U  C  G  A  A  A  U  U  A  G  A  G
// A  A  A  A  C  C  U  U  C  G  G  G
// A  G  A  C  G  C  C  C  G  C  A  G
// C  U  G  G  G  A  A  G  C  C  C  C
// C  G  G  A  A  G  U  C  A  U  G  C
assert_checkequal(vectorfind(m, ["A" "C" "C" "A" "G"], "c"), 6);
assert_checkequal(vectorfind(m, ["C" "U" "G" "G" "G" "A" "A" "G" "C" "C" "C" "C"],"r"), 4);

// Short needle
assert_checkequal(vectorfind(m, ["A" "C" "G"], "c"), [17 21 41 53]);
assert_checkequal(vectorfind(m, ["A" "C" "G"], "r"), 13);
// With joker
[ind, ma] = vectorfind(m, ["A" "" "" "" "A"], "c", "");
assert_checkequal(ind, [4 5 9]);
assert_checkequal(ma, ["A" "A" "C" "G" "A"; "A" "C" "G" "G" "A"; "A" "C" "G" "C" "A"] );
// With joker in short needle
[ind, ma] = vectorfind(m, ["A" "" "U" "" "G"], "r", "");
assert_checkequal(ind, 26);
assert_checkequal(ma, ["A" "U" "U" "A" "G"]);

// Text hypermat:
// -------------
m = matrix(m,3,5,4);
// m  =
//(:,:,1)
//!U  C  A  G  A  !
//!A  C  G  G  G  !
//!A  C  U  A  G  !
//(:,:,2)
//!A  G  C  A  C  !
//!A  A  G  A  A  !
//!C  A  G  C  G  !
//(:,:,3)
//!U  A  U  C  G  !
//!U  U  C  A  C  !
//!C  U  G  C  A  !
//(:,:,4)
//!G  C  G  G  G  !
//!G  U  A  G  C  !
//!C  A  C  G  C  !
assert_checkequal(vectorfind(m, ["A" "A" "C"], "c"), [6 9]);
assert_checkequal(vectorfind(m, ["" "G" "G"], "c", ""), [5 8 19]);
// Joker
[ind, ma] = vectorfind(m, ["" "G" "G"], "c", "","headN");
assert_checkequal(ind, [13 22 55]);
assert_checkequal(ma, ["A" "G" "G"; "C" "G" "G"; "G" "G" "G"]);
res = [
  1  2  1
  1  5  4];
assert_checkequal(vectorfind(m, ["" "C" "C"], "c", "", "headIJK"), res);
// Short needle
res = [
  1  2  1
  2  2  1
  2  5  4];
assert_checkequal(vectorfind(m, ["C" "C"], "c",,"headIJK"), res);
// Short needle with joker
res = [
  1  3  1
  2  2  2];
assert_checkequal(vectorfind(m, ["A" "" "A"],"r","","headIJK"), res);

// =============
// WITH BOOLEANS
// =============
m = asciimat(m(:,:,1:3))<70;
// m  =
// (:,:,1)
//  F T T F T
//  T T F F F
//  T T F T F
// (:,:,2)
//  T F T T T
//  T T F T T
//  T T F T F
// (:,:,3)
//  F T F T F
//  F F T T T
//  T F F T T
assert_checkequal(vectorfind(m, [%T %T %T],"c"), [2 6 9 14]);
// short needle
assert_checkequal(vectorfind(m, [%T %T %T],"r"), [22 38]);
res = [
  1  3  2
  2  3  3];
assert_checkequal(vectorfind(m, [%T %T %T],"r",,"headIJK"), res);
// Joker: Note that v and joker are numerical
[ind, ma] = vectorfind(m, [0 2 1],"c",2);
assert_checkequal(ind, [1 4 7 11 15]);
assert_checkequal(ma, [0 1 1; 0 0 1; 0 1 1; 0 0 1; 0 1 1]==1);
// Short needle with joker
res = [
  1  1  2
  2  2  2
  3  1  3];
assert_checkequal(vectorfind(m, [1 0 2 1],"r",2, "headIJK"), res);
