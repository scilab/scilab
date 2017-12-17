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

// With []
assert_checkequal(circshift([], 1), []);
assert_checkequal(circshift([], 1, 2), []);
assert_checkequal(circshift([], 1, 0), []);

// With a row
assert_checkequal(circshift(1:9, 0), 1:9);
assert_checkequal(circshift(1:9, 1), [9 1:8]);
assert_checkequal(circshift(1:9, [2 1]), [9 1:8]);
assert_checkequal(circshift(1:9, [1 2], [2 1]), [9 1:8]);
assert_checkequal(circshift(1:9, -1), [2:9 1]);
assert_checkequal(circshift(1:9, [2 -1]), [2:9 1]);
assert_checkequal(circshift(1:9, [-1 2], [2 1]), [2:9 1]);
assert_checkequal(circshift(1:9, -2, 0), [3:9 1:2]);
assert_checkequal(circshift(1:9, [2 -2], [2 2]), 1:9);

// With a column
assert_checkequal(circshift((1:9)', 0), (1:9)');
assert_checkequal(circshift((1:9)', 1), [9 1:8]');
assert_checkequal(circshift((1:9)', [1 2]), [9 1:8]');
assert_checkequal(circshift((1:9)', [2 1], [2 1]), [9 1:8]');
assert_checkequal(circshift((1:9)', -1), [2:9 1]');
assert_checkequal(circshift((1:9)', [-1 2]), [2:9 1]');
assert_checkequal(circshift((1:9)', [2 -1], [2 1]), [2:9 1]');
assert_checkequal(circshift((1:9)', -2, 0), [3:9 1:2]');
assert_checkequal(circshift((1:9)', [2 -2], [1 1]), (1:9)');

// With a matrix
M = [1  2  3  4
     5  6  7  8
     9 10 11 12];
assert_checkequal(circshift(M, 0), M);
assert_checkequal(circshift(M, 1),  [9 10 11 12; 1  2  3  4; 5 6 7 8]);
assert_checkequal(circshift(M, -1), [5 6  7  8 ; 9 10 11 12; 1 2 3 4]);
assert_checkequal(circshift(M, [2 -2], [1 1]), M);
assert_checkequal(circshift(M, [2 -2], [2 2]), M);
assert_checkequal(circshift(M, 3), M);
assert_checkequal(circshift(M, -3), M);
assert_checkequal(circshift(M, 4, 2), M);
assert_checkequal(circshift(M, -4, 2), M);
assert_checkequal(circshift(M, [3 4], [1 2]), M);
assert_checkequal(circshift(M, [1 2], [1 2]), [11 12 9 10 ; 3 4 1 2 ; 7 8 5 6]);
assert_checkequal(circshift(M, [-2 -2], [1 2]), [11 12 9 10 ; 3 4 1 2 ; 7 8 5 6]);
assert_checkequal(circshift(M, 1, 0), [12 9 10 11 ; 1 2 3 4 ; 5 6 7 8]);
assert_checkequal(circshift(M, 3, 0), circshift(M,1,2));

// With shifts or/and dims specified as encoded integers:
// -----------------------------------------------------
assert_checkequal(circshift(M, int8(-3)), M);
assert_checkequal(circshift(M, 4, int8(2)), M);
assert_checkequal(circshift(M, int16(-4), uint8(2)), M);
assert_checkequal(circshift(M, uint8([3 4]), uint16([1 2])), M);


// With an hypermatrix of texts
// ============================
t = matrix([string(1:12) strsplit("a":"l")' strsplit("A":"L")'],3,4,3);
// t  =
//(:,:,1)
//!1  4  7  10  !
//!2  5  8  11  !
//!3  6  9  12  !
//
//(:,:,2)
//!a  d  g  j  !
//!b  e  h  k  !
//!c  f  i  l  !
//
//(:,:,3)
//!A  D  G  J  !
//!B  E  H  K  !
//!C  F  I  L  !
// ---------
expected = cat(3, ["L" "3" "6" "9" ; "1" "4" "7" "10"; "2" "5" "8" "11"], ..
                  ["12" "c" "f" "i"; "a" "d" "g" "j" ; "b" "e" "h" "k" ], ..
                  ["l" "C" "F" "I" ; "A" "D" "G" "J" ; "B" "E" "H" "K" ]);
assert_checkequal(circshift(t, 1, 0), expected);
//     ans  =
//    (:,:,1)
//    !L  3  6  9   !
//    !1  4  7  10  !
//    !2  5  8  11  !
//    (:,:,2)
//    !12  c  f  i  !
//    !a   d  g  j  !
//    !b   e  h  k  !
//    (:,:,3)
//    !l  C  F  I  !
//    !A  D  G  J  !
//    !B  E  H  K  !
// ---------
expected = cat(3, ["3" "6" "9" "12" ; "1" "4" "7" "10" ; "2" "5" "8" "11"], ..
                  ["c" "f" "i" "l"  ; "a" "d" "g" "j"  ; "b" "e" "h" "k"], ..
                  ["C" "F" "I" "L"  ; "A" "D" "G" "J"  ; "B" "E" "H" "K"]);
assert_checkequal(circshift(t, 1), expected);
//     ans  =
//    (:,:,1)
//    !3  6  9  12  !
//    !1  4  7  10  !
//    !2  5  8  11  !
//    (:,:,2)
//    !c  f  i  l  !
//    !a  d  g  j  !
//    !b  e  h  k  !
//    (:,:,3)
//    !C  F  I  L  !
//    !A  D  G  J  !
//    !B  E  H  K  !
// ---------
expected = cat(3, ["A" "D" "G" "J" ; "B" "E" "H" "K" ; "C" "F" "I" "L"], ..
                  ["1" "4" "7" "10"; "2" "5" "8" "11"; "3" "6" "9" "12"], ..
                  ["a" "d" "g" "j" ; "b" "e" "h" "k" ; "c" "f" "i" "l"]);
assert_checkequal(circshift(t, 1, 3), expected);
//     ans  =
//    (:,:,1)
//    !A  D  G  J  !
//    !B  E  H  K  !
//    !C  F  I  L  !
//    (:,:,2)
//    !1  4  7  10  !
//    !2  5  8  11  !
//    !3  6  9  12  !
//    (:,:,3)
//    !a  d  g  j  !
//    !b  e  h  k  !
//    !c  f  i  l  !
// ---------
expected = cat(3, ["j"  "a" "d" "g" ; "k" "b" "e" "h" ; "l" "c" "f" "i"], ..
                  ["J"  "A" "D" "G" ; "K" "B" "E" "H" ; "L" "C" "F" "I"], ..
                  ["10" "1" "4" "7"; "11" "2" "5" "8"; "12" "3" "6" "9"]);
assert_checkequal(circshift(t, [1 -1], [2 3]), expected);
//     ans  =
//    (:,:,1)
//    !j  a  d  g  !
//    !k  b  e  h  !
//    !l  c  f  i  !
//    (:,:,2)
//    !J  A  D  G  !
//    !K  B  E  H  !
//    !L  C  F  I  !
//    (:,:,3)
//    !10  1  4  7  !
//    !11  2  5  8  !
//    !12  3  6  9  !

// With a cell array
// =================
L = list(1,%t);
M = {%f "a" %i  %z
     %t "b" %e  %s
      0 "c" %pi L };
assert_checkequal(circshift(M, 0), M);
assert_checkequal(circshift(M, 1), {0 "c" %pi L ; %f "a" %i  %z ; %t "b" %e  %s});
assert_checkequal(circshift(M, -2), {0 "c" %pi L ; %f "a" %i  %z ; %t "b" %e  %s});
assert_checkequal(circshift(M, [2 -2], [1 1]), M);
assert_checkequal(circshift(M, [2 -2], [2 2]), M);
assert_checkequal(circshift(M, 3), M);
assert_checkequal(circshift(M, -3), M);
assert_checkequal(circshift(M, 4, 2), M);
assert_checkequal(circshift(M, -4, 2), M);
assert_checkequal(circshift(M, [3 4], [1 2]), M);
assert_checkequal(circshift(M, [1 2], [1 2]), {%pi L 0 "c" ; %i %z %f "a"; %e %s %t "b"});
assert_checkequal(circshift(M, [-2 -2],[1 2]),{%pi L 0 "c" ; %i %z %f "a"; %e %s %t "b"});

