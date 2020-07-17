// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018-2020 - Samuel Gougeon
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// unit test of unique()
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// =====================
// Checking output sizes
// =====================
objects = list([], 7, 1:5, (1:5)', [1 3 5 ; 2 4 6], matrix(1:12,[2 3 2]));
for o = objects
    [u,ki,ko,nb] = unique(o);
    n = length(o);
    if o==[], su = [0 0]; else su = [n 1]; end
    if isrow(o), su = [1 n]; end
    assert_checkequal(size(u), su);
    assert_checkequal(size(ki), su);
    assert_checkequal(size(ko), size(o));
    assert_checkequal(size(nb), size(ki));
end
objects($) = null(); // Removing hypermat (excluded from "r" and "c" options)
for o = objects
    [u,ki,ko,nb] = unique(o, "r");
    [nr, nc] = size(o);
    if o==[], su = [0 0]; else su = [nr 1]; end
    assert_checkequal(size(u),  size(o));
    assert_checkequal(size(ki), su);
    assert_checkequal(size(ko), su);
    assert_checkequal(size(nb), size(ki));

    [u,ki,ko,nb] = unique(o, "c");
    [nr, nc] = size(o);
    if o==[], su = [0 0]; else su = [1 nc]; end
    assert_checkequal(size(u),  size(o));
    assert_checkequal(size(ki), su);
    assert_checkequal(size(ko), su);
    assert_checkequal(size(nb), size(ki));
end


// ====================
// With decimal numbers
// ====================
// -----
// EMPTY
// -----
// By element
[u,ki,ko,nb] = unique([]);
assert_checkequal(u, []);
assert_checkequal(ki, []);
assert_checkequal(ko, []);
assert_checkequal(nb, []);
// "r"
[u,ki,ko,nb] = unique([], "r");
assert_checkequal(u, []);
assert_checkequal(ki, []);
assert_checkequal(ko, []);
assert_checkequal(nb, []);
// "c"
[u,ki,ko,nb] = unique([], "c");
assert_checkequal(u, []);
assert_checkequal(ki, []);
assert_checkequal(ko, []);
assert_checkequal(nb, []);

// ---
// ROW
// ---
x = [1 3 %nan 3 %inf 4 0 %nan 4 1];

// By elements
[u, ki, ko, nb] = unique(x);
assert_checkequal(u, [0  1  3  4  %inf  %nan  %nan]);
assert_checkequal(ki, [7  1  2  6  5  3  8]);
assert_checkequal(x(ki), u);
assert_checkequal(ko, [2  3  6  3  5  4  1  7  4  2]);
assert_checkequal(u(ko), x);
assert_checkequal(nb, [1  2  2  2  1  1  1]);
// "r"
[u,ki,ko,nb] = unique(x, "r");
assert_checkequal(u, x);
assert_checkequal(ki, 1);
assert_checkequal(x(ki,:), u);
assert_checkequal(ko, 1);
assert_checkequal(u(ko,:), x);
assert_checkequal(nb, 1);
// "c"
[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, [0  1  3  4  %inf  %nan  %nan]);
assert_checkequal(ki, [7  1  2  6  5  3  8]);
assert_checkequal(x(:,ki), u);
assert_checkequal(ko, [2  3  6  3  5  4  1  7  4  2]);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb, [1  2  2  2  1  1  1]);

// ------
// COLUMN
// ------
x = x';
// By elements
[u, ki, ko, nb] = unique(x);
assert_checkequal(u, [0  1  3  4  %inf  %nan  %nan]');
assert_checkequal(ki, [7  1  2  6  5  3  8]');
assert_checkequal(x(ki), u);
assert_checkequal(ko, [2  3  6  3  5  4  1  7  4  2]');
assert_checkequal(u(ko), x);
assert_checkequal(nb, [1  2  2  2  1  1  1]');
// "r"
[u,ki,ko,nb] = unique(x, "r");
assert_checkequal(u, [0  1  3  4  %inf  %nan  %nan]');
assert_checkequal(ki, [7  1  2  6  5  3  8]');
assert_checkequal(x(ki,:), u);
assert_checkequal(ko, [2  3  6  3  5  4  1  7  4  2]');
assert_checkequal(u(ko,:), x);
assert_checkequal(nb, [1  2  2  2  1  1  1]');
// "c"
[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, x);
assert_checkequal(ki, 1);
assert_checkequal(x(:,ki), u);
assert_checkequal(ko, 1);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb, 1);

// ------
// MATRIX
// ------
x = [ 2  0  2  2  2  0  0  0  2  0  0  2  2  1  0  1
      1  2  2  2  1  0  2  2  0  0  1  0  1  1  0  0
    ];
u = unique(x);
assert_checkequal(u, [0 1 2]');
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, [0 1 2]');
assert_checkequal(ki, [3 2 1]');
assert_checkequal(ko, x+1);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[13 7 12]');
// "c"
u = unique(x, "c");
uxc = [ 0  0  0  1  1  2  2  2
        0  1  2  0  1  0  1  2
      ];
assert_checkequal(u, uxc);
[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, uxc);
assert_checkequal(ki, [6  11  2  16  14  9  1  3]);
assert_checkequal(x(:,ki), u);
assert_checkequal(ko, [7 3 8 8 7 1 3 3 6 1 2 6 7 5 1 4]);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb, [3  1  3  1  1  2  3  2]);
// "r"
x = x';
u = unique(x, "r");
uxc = [ 0  0  0  1  1  2  2  2
        0  1  2  0  1  0  1  2
      ]';
assert_checkequal(u, uxc);
[u,ki,ko,nb] = unique(x, "r");
assert_checkequal(u, uxc);
assert_checkequal(ki, [6  11  2  16  14  9  1  3]');
assert_checkequal(x(ki,:), u);
assert_checkequal(ko, [7 3 8 8 7 1 3 3 6 1 2 6 7 5 1 4]');
assert_checkequal(u(ko,:), x);
assert_checkequal(nb, [3  1  3  1  1  2  3  2]');

// -----------
// HYPERMATRIX
// -----------
x = cat(3,[2 0 %nan 1 ; 2 1 0 2], [%inf 2 1 2 ; 1 0 %nan 2]);
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, [0 1 2 %inf %nan %nan]');
assert_checkequal(ki, [3  4  1  9  5  14]');
assert_checkequal(u(ko), x);
assert_checkequal(nb, [3  4  6  1  1  1]');
msg = "unique: Argument #2: ''r'' not allowed for an hypermatrix.";
assert_checkerror("unique(x, ""r"")", msg);
msg = "unique: Argument #2: ''c'' not allowed for an hypermatrix.";
assert_checkerror("unique(x, ""c"")", msg);

// ---------
// keepOrder
// ---------
x = [ 2  0  2  2  2  0  0  0  2  0  0  2  2  1  0  1
      1  2  2  2  1  0  2  2  0  0  1  0  1  1  0  0
    ];
[u,ki,ko,nb] = unique(x, "keepOrder");
assert_checkequal(u, [2 1 0]');
assert_checkequal(ki, [1 2 3]');
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[12 7 13]');

uxc = [2  0  2  0  2  0  1  1
       1  2  2  0  0  1  1  0
      ];
// "r"
[u,ki,ko,nb] = unique(x', "r", "keepOrder");
assert_checkequal(u, uxc');
assert_checkequal(ki, [1  2  3  6  9  11 14 16]');
assert_checkequal(x'(ki,:), u);
assert_checkequal(u(ko,:), x');
assert_checkequal(nb, [3  3  2  3  2  1  1  1 ]');
// "c"
[u,ki,ko,nb] = unique(x, "c", "keepOrder");
assert_checkequal(u, uxc);
assert_checkequal(ki, [1  2  3  6  9  11 14 16]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb, [3  3  2  3  2  1  1  1 ]);


// --------------
// More with Nans
// --------------
Nan = %nan; Inf = %inf;
x = [Nan  Nan  Inf  Nan  0  Inf  Inf  Nan  Inf  Nan
     Inf  0    Nan  0    0  0    Nan  Nan  0    0
    ];
u = unique(x);
assert_checkequal(u, [0 Inf Nan*ones(1,8)]');
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, [0 Inf Nan*ones(1,8)]');
assert_checkequal(ki, [4 2 1 3 6 7 14 15 16 19]');
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[7 5 1 1 1 1 1  1  1  1]');
//
u = unique(x, "c");
uxc = [0  Inf  Inf  Inf  Nan  Nan  Nan  Nan  Nan
       0  0    Nan  Nan  0    0    0    Inf  Nan
      ];
assert_checkequal(u, uxc);
[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, uxc);
assert_checkequal(ki, [5  6  3  7  2  4 10  1  8]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb,[1  2  1  1  1  1  1  1  1]);

u = unique(x', "r");
assert_checkequal(u, uxc');
[u,ki,ko,nb] = unique(x', "r");
assert_checkequal(u, uxc');
assert_checkequal(ki, [5  6  3  7  2  4 10  1  8]');
assert_checkequal(x'(ki,:), u);
assert_checkequal(u(ko,:), x');
assert_checkequal(nb,[1  2  1  1  1  1  1  1  1]');

// With Nans & "uniqueNan" option
// ------------------------------
[u,ki,ko,nb] = unique(x, "uniqueNan");
assert_checkequal(u, [0 Inf Nan]');
assert_checkequal(ki, [4 2 1]');
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[7 5 8]');
//
uxc = [0  Inf  Inf  Nan  Nan  Nan
       0  0    Nan  0    Inf  Nan
      ];
[u,ki,ko,nb] = unique(x, "c", "uniqueNan");
assert_checkequal(u, uxc);
assert_checkequal(ki, [5  6  3  2  1  8]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb,[1  2  2  3  1  1]);

[u,ki,ko,nb] = unique(x', "r", "uniqueNan");
assert_checkequal(u, uxc');
assert_checkequal(ki, [5  6  3  2  1  8]');
assert_checkequal(x'(ki,:), u);
assert_checkequal(u(ko,:), x');
assert_checkequal(nb,[1  2  2  3  1  1]');

// With Nans & "uniqueNan" & "keepOrder" options
// ---------------------------------------------
[u,ki,ko,nb] = unique(x, "uniqueNan", "keepOrder");
assert_checkequal(u, [Nan Inf 0]');
assert_checkequal(ki, [1 2 4]');
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[8 5 7]');
//
uxc = [Nan  Nan  Inf  0  Inf  Nan
       Inf  0    Nan  0  0    Nan
      ];
//x = [Nan  Nan  Inf  Nan  0  Inf  Inf  Nan  Inf  Nan
//     Inf  0    Nan  0    0  0    Nan  Nan  0    0
[u,ki,ko,nb] = unique(x, "c", "uniqueNan", "keepOrder");
assert_checkequal(u, uxc);
assert_checkequal(ki, [1  2  3  5  6  8]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb,[1  3  2  1  2  1]);

[u,ki,ko,nb] = unique(x', "r", "uniqueNan", "keepOrder");
assert_checkequal(u, uxc');
assert_checkequal(ki, [1  2  3  5  6  8]');
assert_checkequal(x'(ki,:), u);
assert_checkequal(u(ko,:), x');
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
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, int8([-3 0 2])');
assert_checkequal(ki, [1 4 6]');
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[6 5 4]');

u = unique(x, "r");
assert_checkequal(u, int8([-3 0 2 0 -3 ; -3 2 0 2 -3]));
[u,ki,ko,nb] = unique(x, "r");
assert_checkequal(u, int8([-3 0 2 0 -3 ; -3 2 0 2 -3]));
assert_checkequal(ki, [1;3]);
assert_checkequal(x(ki,:), u);
assert_checkequal(u(ko,:), x);
assert_checkequal(nb,[2;1]);

u = unique(x, "c");
assert_checkequal(u, int8([-3 0 2 ; -3 0 2; -3 2 0]));
[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, int8([-3 0 2 ; -3 0 2; -3 2 0]));
assert_checkequal(ki, [1 2 3]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb,[2 2 1]);

// =============
// With booleans
// =============
x = [%F %T %F %F %F %T %T
     %F %T %T %F %T %F %F ];
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, [%F ; %T]);
assert_checkequal(ki, [1;3]);
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[8;6]);

[u,ki,ko,nb] = unique(x, "c");
assert_checkequal(u, [%F %F %T %T ; %F %T %F %T]);
assert_checkequal(ki, [1 3 6 2]);
assert_checkequal(x(:,ki), u);
assert_checkequal(u(:,ko), x);
assert_checkequal(nb,[2 2 2 1]);



// ====================
// With complex numbers
// ====================
i = %i;
x = [1-i, 3+2*i, 1+i, 3-2*i, 3+2*i, 3-i, 1-i];
assert_checkequal(unique(x), [1-i, 1+i, 3-i, 3-2*i, 3+2*i]);
[u,ki,ko,nb] = unique(x);
assert_checkequal(u, [1-i, 1+i, 3-i, 3-2*i, 3+2*i]);
assert_checkequal(ki, [1  3  6  4  2]);
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[2  1  1  1  2]);

c = [x($:-1:1) ; x ; x($:-1:1) ; 1:7 ; x];
u = unique(c, "r");
assert_checkequal(u, c([4 1 2],:));
[u,ki,ko,nb] = unique(c, "r");
assert_checkequal(u, c([4 1 2],:));
assert_checkequal(ki, [4 1 2]');
assert_checkequal(c(ki,:), u);
assert_checkequal(u(ko,:), c);
assert_checkequal(nb,[1 2 2]');

c = c.';
u = unique(c, "c");
kref = [4 1 2];
assert_checkequal(u, c(:,kref));
[u,ki,ko,nb] = unique(c, "c");
assert_checkequal(u, c(:,kref));
assert_checkequal(ki, kref);
assert_checkequal(c(:,ki), u);
assert_checkequal(u(:,ko), c);
assert_checkequal(nb,[1 2 2]);

// ---------
// keepOrder
// ---------
// ROW
[u,ki,ko,nb] = unique(x, "keepOrder");
assert_checkequal(u, [1-i, 3+2*i, 1+i, 3-2*i, 3-i]);
assert_checkequal(ki, [1  2  3  4  6]);
assert_checkequal(x(ki), u);
assert_checkequal(u(ko), x);
assert_checkequal(nb,[2  2 1  1  1 ]);

c = [x($:-1:1) ; x ; x($:-1:1) ; 1:7 ; x];
kref = [1 2 4];
[u,ki,ko,nb] = unique(c, "r", "keepOrder");
assert_checkequal(u, c(kref,:));
assert_checkequal(ki, kref');
assert_checkequal(c(ki,:), u);
assert_checkequal(u(ko,:), c);
assert_checkequal(nb,[2 2 1]');

c = c.';
[u,ki,ko,nb] = unique(c, "c", "keepOrder");
assert_checkequal(u, c(:,kref));
assert_checkequal(ki, kref);
assert_checkequal(c(:,ki), u);
assert_checkequal(u(:,ko), c);
assert_checkequal(nb, [2 2 1]);

// ==============
// With some text
// ==============
t = ["AB" "BA" "BA" "BA" "AB" "BA" "AB" "AB" "BB" "AA" "AB" "BA" "BA" "BA" "AA"
     "AA" "AA" "AB" "AA" "BB" "BB" "BB" "BA" "AB" "AB" "BB" "BB" "AB" "AB" "AA"
    ];
u = unique(t);
assert_checkequal(u, ["AA" "AB" "BA" "BB"]');
[u,ki,ko,nb] = unique(t);
assert_checkequal(u, ["AA" "AB" "BA" "BB"]');
assert_checkequal(ki, [2 1 3 10]');
assert_checkequal(t(ki), u);
assert_checkequal(u(ko), t);
assert_checkequal(nb,[6 10 8 6]');

utc = ["AA"  "AA"  "AB"  "AB"  "AB"  "BA"  "BA"  "BA"  "BB"
       "AA"  "AB"  "AA"  "BA"  "BB"  "AA"  "AB"  "BB"  "AB"
      ];
u = unique(t, "c");
assert_checkequal(u, utc);
[u,ki,ko,nb] = unique(t, "c");
assert_checkequal(u, utc);
assert_checkequal(ki, [15 10 1 8 5 2 3 6 9]);
assert_checkequal(t(:,ki), u);
assert_checkequal(u(:,ko), t);
assert_checkequal(nb,[1  1  1 1 3 2 3 2 1]);

u = unique(t', "r");
assert_checkequal(u, utc');
[u,ki,ko,nb] = unique(t', "r");
assert_checkequal(u, utc');
assert_checkequal(ki, [15 10 1 8 5 2 3 6 9]');
assert_checkequal(t'(ki,:), u);
assert_checkequal(u(ko,:), t');
assert_checkequal(nb,[1  1  1 1 3 2 3 2 1]');

// keepOrder
// ---------
[u,ki,ko,nb] = unique(t, "keepOrder");
assert_checkequal(u, ["AB" "AA" "BA" "BB"]');
assert_checkequal(ki, [1 2 3 10]');
assert_checkequal(t(ki), u);
assert_checkequal(u(ko), t);
assert_checkequal(nb,[10 6 8 6]');

utc = ["AB"  "BA"  "BA"  "AB"  "BA"  "AB"  "BB"  "AA"  "AA"
       "AA"  "AA"  "AB"  "BB"  "BB"  "BA"  "AB"  "AB"  "AA"
      ];
[u,ki,ko,nb] = unique(t, "c", "keepOrder");
assert_checkequal(u, utc);
assert_checkequal(ki, [1 2 3 5 6 8 9 10 15]);
assert_checkequal(t(:,ki), u);
assert_checkequal(u(:,ko), t);
assert_checkequal(nb,[1 2 3 3 2 1 1 1  1 ]);

[u,ki,ko,nb] = unique(t', "r", "keepOrder");
assert_checkequal(u, utc');
assert_checkequal(ki, [1 2 3 5 6 8 9 10 15]');
assert_checkequal(t'(ki,:), u);
assert_checkequal(u(ko,:), t');
assert_checkequal(nb,[1 2 3 3 2 1 1 1  1 ]');
