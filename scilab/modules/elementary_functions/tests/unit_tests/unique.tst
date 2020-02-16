// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018-2019 - Samuel Gougeon
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// unit test of unique()
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// =====================
// Checking output sizes
// =====================
// "*"
[u,k,?,nb] = unique([]);
assert_checkequal(u, []);
assert_checkequal(k, []);
assert_checkequal(nb,[]);
[u,k,?,nb] = unique([1 4 5]);
sref = [1 3];
assert_checkequal(size(u), sref);
assert_checkequal(size(k), sref);
assert_checkequal(size(nb),sref);
[u,k,?,nb] = unique([1 4 5]');
sref = [3 1];
assert_checkequal(size(u), sref);
assert_checkequal(size(k), sref);
assert_checkequal(size(nb),sref);
[u,k,?,nb] = unique([1 3 4 ; 5 6 7]);
sref = [6 1];
assert_checkequal(size(u), sref);
assert_checkequal(size(k), sref);
assert_checkequal(size(nb),sref);
[u,k,?,nb] = unique(rand(3,4,2));
sref = [24 1];
assert_checkequal(size(u), sref);
assert_checkequal(size(k), sref);
assert_checkequal(size(nb),sref);

// "r"
[u,k,?,nb] = unique([], "r");
assert_checkequal(u, []);
assert_checkequal(k, []);
assert_checkequal(nb,[]);
[u,k,?,nb] = unique([1 3 4 ; 5 6 7], "r");
assert_checkequal(size(u), [2 3]);
assert_checkequal(size(k), [2 1]);
assert_checkequal(size(nb),[2 1]);

// "c"
[u,k,?,nb] = unique([], "c");
assert_checkequal(u,[]);
assert_checkequal(k,[]);
[u,k,?,nb] = unique([1 3 4 ; 5 6 7], "c");
assert_checkequal(size(u), [2 3]);
assert_checkequal(size(k), [1 3]);
assert_checkequal(size(nb),[1 3]);

// ====================
// With decimal numbers
// ====================
x = [ 2  0  2  2  2  0  0  0  2  0  0  2  2  1  0  1
      1  2  2  2  1  0  2  2  0  0  1  0  1  1  0  0
    ];
u = unique(x);
assert_checkequal(u, [0 1 2]');
[u,k,?,nb] = unique(x);
assert_checkequal(u, [0 1 2]');
assert_checkequal(k, [3 2 1]');
assert_checkequal(nb,[13 7 12]');

u = unique(x, "c");
uxc = [ 0  0  0  1  1  2  2  2
        0  1  2  0  1  0  1  2
      ];
assert_checkequal(u, uxc);
[u,k,?,nb] = unique(x, "c");
assert_checkequal(u, uxc);
assert_checkequal(k, [6  11  2  16  14  9  1  3]);
assert_checkequal(nb,[3  1   3  1   1   2  3  2]);

u = unique(x', "r");
assert_checkequal(u, uxc');
[u,k,?,nb] = unique(x', "r");
assert_checkequal(u, uxc');
assert_checkequal(k, [6  11  2  16  14  9  1  3]');
assert_checkequal(nb,[3  1   3  1   1   2  3  2]');

// keepOrder
// ---------
[u,k,?,nb] = unique(x, "keepOrder");
assert_checkequal(u, [2 1 0]');
assert_checkequal(k, [1 2 3]');
assert_checkequal(nb,[12 7 13]');

[u,k,?,nb] = unique(x, "c", "keepOrder");
uxc = [2  0  2  0  2  0  1  1
       1  2  2  0  0  1  1  0
      ];
assert_checkequal(u, uxc);
assert_checkequal(k, [1  2  3  6  9  11 14 16]);
assert_checkequal(nb,[3  3  2  3  2  1  1  1 ]);

[u,k,?,nb] = unique(x', "r", "keepOrder");
assert_checkequal(u, uxc');
assert_checkequal(k, [1  2  3  6  9  11 14 16]');
assert_checkequal(nb,[3  3  2  3  2  1  1  1 ]');

// With Nans
// ---------
Nan = %nan; Inf = %inf;
x = [Nan  Nan  Inf  Nan  0  Inf  Inf  Nan  Inf  Nan
     Inf  0    Nan  0    0  0    Nan  Nan  0    0
    ];
u = unique(x);
assert_checkequal(u, [0 Inf Nan*ones(1,8)]');
[u,k,?,nb] = unique(x);
assert_checkequal(u, [0 Inf Nan*ones(1,8)]');
assert_checkequal(k, [4 2 1 3 6 7 14 15 16 19]');
assert_checkequal(nb,[7 5 1 1 1 1 1  1  1  1]');
//
u = unique(x, "c");
uxc = [0  Inf  Inf  Inf  Nan  Nan  Nan  Nan  Nan
       0  0    Nan  Nan  0    0    0    Inf  Nan
      ];
assert_checkequal(u, uxc);
[u,k,?,nb] = unique(x, "c");
assert_checkequal(u, uxc);
assert_checkequal(k, [5  6  3  7  2  4 10  1  8]);
assert_checkequal(nb,[1  2  1  1  1  1  1  1  1]);

u = unique(x', "r");
assert_checkequal(u, uxc');
[u,k,?,nb] = unique(x', "r");
assert_checkequal(u, uxc');
assert_checkequal(k, [5  6  3  7  2  4 10  1  8]');
assert_checkequal(nb,[1  2  1  1  1  1  1  1  1]');

// With Nans & "uniqueNan" option
// ------------------------------
[u,k,?,nb] = unique(x, "uniqueNan");
assert_checkequal(u, [0 Inf Nan]');
assert_checkequal(k, [4 2 1]');
assert_checkequal(nb,[7 5 8]');
//
uxc = [0  Inf  Inf  Nan  Nan  Nan
       0  0    Nan  0    Inf  Nan
      ];
[u,k,?,nb] = unique(x, "c", "uniqueNan");
assert_checkequal(u, uxc);
assert_checkequal(k, [5  6  3  2  1  8]);
assert_checkequal(nb,[1  2  2  3  1  1]);

[u,k,?,nb] = unique(x', "r", "uniqueNan");
assert_checkequal(u, uxc');
assert_checkequal(k, [5  6  3  2  1  8]');
assert_checkequal(nb,[1  2  2  3  1  1]');

// With Nans & "uniqueNan" & "keepOrder" options
// ---------------------------------------------
[u,k,?,nb] = unique(x, "uniqueNan", "keepOrder");
assert_checkequal(u, [Nan Inf 0]');
assert_checkequal(k, [1 2 4]');
assert_checkequal(nb,[8 5 7]');
//
uxc = [Nan  Nan  Inf  0  Inf  Nan
       Inf  0    Nan  0  0    Nan
      ];
//x = [Nan  Nan  Inf  Nan  0  Inf  Inf  Nan  Inf  Nan
//     Inf  0    Nan  0    0  0    Nan  Nan  0    0
[u,k,?,nb] = unique(x, "c", "uniqueNan", "keepOrder");
assert_checkequal(u, uxc);
assert_checkequal(k, [1  2  3  5  6  8]);
assert_checkequal(nb,[1  3  2  1  2  1]);

[u,k,?,nb] = unique(x', "r", "uniqueNan", "keepOrder");
assert_checkequal(u, uxc');
assert_checkequal(k, [1  2  3  5  6  8]');
assert_checkequal(nb,[1  3  2  1  2  1]');

// =====================
// With encoded integers
// =====================
x = int8([-3 0 2 0 -3
          -3 0 2 0 -3
          -3 2 0 2 -3
         ]);
u = unique(x);
assert_checkequal(u, int8([-3 0 2])');
[u,k,?,nb] = unique(x);
assert_checkequal(u, int8([-3 0 2])');
assert_checkequal(k, [1 4 6]');
assert_checkequal(nb,[6 5 4]');

u = unique(x, "r");
assert_checkequal(u, int8([-3 0 2 0 -3 ; -3 2 0 2 -3]));
[u,k,?,nb] = unique(x, "r");
assert_checkequal(u, int8([-3 0 2 0 -3 ; -3 2 0 2 -3]));
assert_checkequal(k, [1;3]);
assert_checkequal(nb,[2;1]);

u = unique(x, "c");
assert_checkequal(u, int8([-3 0 2 ; -3 0 2; -3 2 0]));
[u,k,?,nb] = unique(x, "c");
assert_checkequal(u, int8([-3 0 2 ; -3 0 2; -3 2 0]));
assert_checkequal(k, [1 2 3]);
assert_checkequal(nb,[2 2 1]);


// ====================
// With complex numbers
// ====================
i = %i;
x = [1-i, 3+2*i, 1+i, 3-2*i, 3+2*i, 3-i, 1-i];
assert_checkequal(unique(x), [1-i, 1+i, 3-2*i, 3-i, 3+2*i]);
[u,k,?,nb] = unique(x);
assert_checkequal(u, [1-i, 1+i, 3-2*i, 3-i, 3+2*i]);
assert_checkequal(k, [1  3  4  6  2]);
assert_checkequal(nb,[2  1  1  1  2]);

c = [x($:-1:1) ; x ; x($:-1:1) ; 1:7 ; x];
u = unique(c, "r");
assert_checkequal(u, c([4 2 1],:));
[u,k,?,nb] = unique(c, "r");
assert_checkequal(u, c([4 2 1],:));
assert_checkequal(k, [4 2 1]');
assert_checkequal(nb,[1 2 2]');

u = unique(c', "c");
kref = [4 2 1];
assert_checkequal(u, c'(:,kref));
[u,k,?,nb] = unique(c', "c");
assert_checkequal(u, c'(:,kref));
assert_checkequal(k, kref);
assert_checkequal(nb,[1 2 2]);

// keepOrder
// ---------
[u,k,?,nb] = unique(x, "keepOrder");
assert_checkequal(u, [1-i, 3+2*i, 1+i, 3-2*i, 3-i]);
assert_checkequal(k, [1  2  3  4  6]);
assert_checkequal(nb,[2  2 1  1  1 ]);

c = [x($:-1:1) ; x ; x($:-1:1) ; 1:7 ; x];
kref = [1 2 4];
[u,k,?,nb] = unique(c, "r", "keepOrder");
assert_checkequal(u, c(kref,:));
assert_checkequal(k, kref');
assert_checkequal(nb,[2 2 1]');

[u,k,?,nb] = unique(c', "c", "keepOrder");
assert_checkequal(u, c'(:,kref));
assert_checkequal(k, kref);
assert_checkequal(nb,[2 2 1]);


// ==============
// With some text
// ==============
t = ["AB" "BA" "BA" "BA" "AB" "BA" "AB" "AB" "BB" "AA" "AB" "BA" "BA" "BA" "AA"
     "AA" "AA" "AB" "AA" "BB" "BB" "BB" "BA" "AB" "AB" "BB" "BB" "AB" "AB" "AA"
    ];
u = unique(t);
assert_checkequal(u, ["AA" "AB" "BA" "BB"]');
[u,k,?,nb] = unique(t);
assert_checkequal(u, ["AA" "AB" "BA" "BB"]');
assert_checkequal(k, [2 1 3 10]');
assert_checkequal(nb,[6 10 8 6]');

utc = ["AA"  "AA"  "AB"  "AB"  "AB"  "BA"  "BA"  "BA"  "BB"
       "AA"  "AB"  "AA"  "BA"  "BB"  "AA"  "AB"  "BB"  "AB"
      ];
u = unique(t, "c");
assert_checkequal(u, utc);
[u,k,?,nb] = unique(t, "c");
assert_checkequal(u, utc);
assert_checkequal(k, [15 10 1 8 5 2 3 6 9]);
assert_checkequal(nb,[1  1  1 1 3 2 3 2 1]);

u = unique(t', "r");
assert_checkequal(u, utc');
[u,k,?,nb] = unique(t', "r");
assert_checkequal(u, utc');
assert_checkequal(k, [15 10 1 8 5 2 3 6 9]');
assert_checkequal(nb,[1  1  1 1 3 2 3 2 1]');

// keepOrder
// ---------
[u,k,?,nb] = unique(t, "keepOrder");
assert_checkequal(u, ["AB" "AA" "BA" "BB"]');
assert_checkequal(k, [1 2 3 10]');
assert_checkequal(nb,[10 6 8 6]');

utc = ["AB"  "BA"  "BA"  "AB"  "BA"  "AB"  "BB"  "AA"  "AA"
       "AA"  "AA"  "AB"  "BB"  "BB"  "BA"  "AB"  "AB"  "AA"
      ];
[u,k,?,nb] = unique(t, "c", "keepOrder");
assert_checkequal(u, utc);
assert_checkequal(k, [1 2 3 5 6 8 9 10 15]);
assert_checkequal(nb,[1 2 3 3 2 1 1 1  1 ]);

[u,k,?,nb] = unique(t', "r", "keepOrder");
assert_checkequal(u, utc');
assert_checkequal(k, [1 2 3 5 6 8 9 10 15]');
assert_checkequal(nb,[1 2 3 3 2 1 1 1  1 ]');
