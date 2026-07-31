// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for squareform function
// =============================================================================

D = 0;
v = squareform(D);
assert_checkequal(v, zeros(2,2));

v = [];
D = squareform(v);
assert_checkequal(D, []);

v = [1 2 3];
D = squareform(v);
D_ref = [0 1 2;
         1 0 3;
         2 3 0];
assert_checkequal(D, D_ref);

v2 = squareform(D);
assert_checkequal(v2, v);

X = [0 0;
     1 0;
     0 1];

D = pdist(X);
v = squareform(D);
D2 = squareform(v);
assert_checkequal(D2, D);

X = [1 2 10 3 9 8];
s = squareform(X);
assert_checkequal(s, [0 1 2 10; 1 0 3 9; 2 3 0 8; 10 9 8 0]);
X2 = squareform(s);
assert_checkequal(X2, X);

n = 10;
X = rand(n, 3);

D = pdist(X);
v = squareform(D);
D2 = squareform(v);

assert_checkequal(D2, D);

// --- tomatrix ---
v = [1 2 3];
D = squareform(v, "tomatrix");
D_ref = [0 1 2;
         1 0 3;
         2 3 0];
assert_checkequal(D, D_ref);

// --- tovector ---
D = [0 4 5;
     4 0 6;
     5 6 0];
v = squareform(D, "tovector");

v_ref = [4 5 6];
assert_checkequal(v, v_ref);

// check errors
v = [1 2 3 4];
msg = msprintf(_("%s: Wrong length for input argument #%d: Cannot form a square matrix from %d elements.\n"), "squareform", 1, 4);
assert_checkerror("squareform(v)", msg);

D = [1 2 3;
     4 5 6];
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be a square matrix.\n"), "squareform", 1);
assert_checkerror("squareform(D)", msg);
