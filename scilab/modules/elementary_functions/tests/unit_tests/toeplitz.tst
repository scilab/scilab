// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// With []
assert_checkequal(toeplitz([]), []);
assert_checkequal(toeplitz([],[]), []);

// With booleans
expected = [1  0  0  0  1  1
            0  1  0  0  0  1
            1  0  1  0  0  0
           ]==1;
assert_checkequal(toeplitz([%t %f %t], [%t %f %f %f %t %t]), expected);

// With encoded integers
expected = int8([-1  0  1  2 ; 0 -1  0  1 ; 1  0 -1  0 ; 2  1  0 -1]);
assert_checkequal(toeplitz(int8(-1:2)), expected);

// With decimal numbers (dense)
expected = [-1  0  1  2 ; 0 -1  0  1 ; 1  0 -1  0 ; 2  1  0 -1];
assert_checkequal(toeplitz(-1:2), expected);

r = 1:5;
c = [1 6:9];
toe = [1 6 7 8 9;
2 1 6 7 8;
3 2 1 6 7;
4 3 2 1 6;
5 4 3 2 1];
assert_checkequal(toeplitz(r, c), toe);

// With sparse
expected = [
  0 -1 -2  0  0  0
  1  0 -1 -2  0  0
  0  1  0 -1 -2  0
  0  0  1  0 -1 -2
   ];
T = toeplitz([0 1 0 0 ], sparse([0 -1 -2 0 0 0]));
assert_checktrue(type(T)==5);
assert_checkequal(full(T), expected);

T = toeplitz(sparse([0 1 0 0 ]), [0 -1 -2 0 0 0]);
assert_checktrue(type(T)==5);
assert_checkequal(full(T), expected);

T = toeplitz(sparse([0 1 0 0 ]), sparse([0 -1 -2 0 0 0]));
assert_checktrue(type(T)==5);
assert_checkequal(full(T), expected);

// With polynomials
expected = [1     2    3   4   5
           %s     1    2   3   4
           %s^2  %s    1   2   3
           %s^3 %s^2  %s   1   2
           ];
assert_checkequal(toeplitz([1 %s %s^2 %s^3], [1:5]), expected);

// With rationals
expected = 1 ./ %s.^toeplitz([0 1],0:3);
assert_checkequal(toeplitz([1 1/%s], [1 1/%s 1/%s^2 1/%s^3]), expected);

// With texts
expected = [
    "-" "a" "b" "c" "d" "e"
    "A" "-" "a" "b" "c" "d"
    "B" "A" "-" "a" "b" "c"
    "C" "B" "A" "-" "a" "b"
    ];
T = toeplitz(["-" "A" "B" "C"], ["-" "a" "b" "c" "d" "e"]);
assert_checkequal(T, expected);
