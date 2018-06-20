// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel Gougeon
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// unit test of unique()
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Checking output sizes
// ---------------------
// "*"
[u,k] = unique([]);
assert_checkequal(u,[]);
assert_checkequal(k,[]);
[u,k] = unique([1 4 5]);
assert_checkequal(size(u),[1 3]);
assert_checkequal(size(k),[1 3]);
[u,k] = unique([1 4 5]');
assert_checkequal(size(u),[3 1]);
assert_checkequal(size(k),[3 1]);
[u,k] = unique([1 4 5]');
assert_checkequal(size(u),[3 1]);
assert_checkequal(size(k),[3 1]);
[u,k] = unique([1 3 4 ; 5 6 7]);
assert_checkequal(size(u),[6 1]);
assert_checkequal(size(k),[6 1]);
[u,k] = unique([1 3 4 ; 5 6 7]);
assert_checkequal(size(u),[6 1]);
assert_checkequal(size(k),[6 1]);
[u,k] = unique(rand(3,4,2));
assert_checkequal(size(u),[24 1]);
assert_checkequal(size(k),[24 1]);

// "r"
[u,k] = unique([], "r");
assert_checkequal(u,[]);
assert_checkequal(k,[]);
[u,k] = unique([1 3 4 ; 5 6 7], "r");
assert_checkequal(size(u),[2 3]);
assert_checkequal(size(k),[2 1]);

// "c"
[u,k] = unique([], "c");
assert_checkequal(u,[]);
assert_checkequal(k,[]);
[u,k] = unique([1 3 4 ; 5 6 7], "c");
assert_checkequal(size(u),[2 3]);
assert_checkequal(size(k),[1 3]);

// With decimal numbers
// --------------------
x = [ 2  0  2  2  2  0  0  0  2  0  0  2  2  1  0  1
      1  2  2  2  1  0  2  2  0  0  1  0  1  1  0  0
    ];
[u, k] = unique(x);
assert_checkequal(u, [0 1 2]');
assert_checkequal(k, [3 2 1]');
[u, k] = unique(x, "c");
uxc = [ 0  0  0  1  1  2  2  2
        0  1  2  0  1  0  1  2
      ];
assert_checkequal(u, uxc);
assert_checkequal(k, [6  11  2  16  14  9  1  3]);

// With encoded integers
// ---------------------
x = int8([-3 0 2 0 -3
          -3 0 2 0 -3
          -3 2 0 2 -3
         ]);
[u,k] = unique(x);
assert_checkequal(u, int8([-3 0 2])');
assert_checkequal(k, [1 4 6]');
[u,k] = unique(x, "r");
assert_checkequal(u, int8([-3 0 2 0 -3 ; -3 2 0 2 -3]));
assert_checkequal(k, [1;3]);
[u,k] = unique(x, "c");
assert_checkequal(u, int8([-3 0 2 ; -3 0 2; -3 2 0]));
assert_checkequal(k, [1 2 3]);

// With complex numbers
// --------------------
i = %i;
x = [1-i, 3+2*i, 1+i, 3-2*i, 3+2*i, 3-%i, 1-i];
assert_checkequal(unique(x), [1-i, 1+i, 3-2*i, 3-i, 3+2*i]);
c = [x($:-1:1) ; x ; x($:-1:1) ; 1:7 ; x];
[u, k] = unique(c, "r");
assert_checkequal(u, c([4 2 1],:));
[u, k] = unique(c', "c");
assert_checkequal(u, c'(:,[4 2 1]));
 
// With some text
// --------------
t = ["AB" "BA" "BA" "BA" "AB" "BA" "AB" "AB" "BB" "AA" "AB" "BA" "BA" "BA" "AA"
     "AA" "AA" "AB" "AA" "BB" "BB" "BB" "BA" "AB" "AB" "BB" "BB" "AB" "AB" "AA"
    ];
[u, k] = unique(t);
assert_checkequal(u, ["AA" "AB" "BA" "BB"]');
assert_checkequal(k, [2 1 3 10]');

utc = ["AA"  "AA"  "AB"  "AB"  "AB"  "BA"  "BA"  "BA"  "BB"
       "AA"  "AB"  "AA"  "BA"  "BB"  "AA"  "AB"  "BB"  "AB"
      ];
[u, k] = unique(t, "c");
assert_checkequal(u, utc);
assert_checkequal(k, [15 10 1 8 5 2 3 6 9]);
[u, k] = unique(t', "r");
assert_checkequal(u, utc');
assert_checkequal(k, [15 10 1 8 5 2 3 6 9]');
