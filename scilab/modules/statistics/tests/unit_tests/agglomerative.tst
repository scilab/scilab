// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for agglomerative function
// =============================================================================
X = [0 0; 0.2 0.1; -0.1 0.3; 0.3 -0.2; 0.1 0.4;
    5 5; 5.2 5.1; 4.9 5.3; 5.3 4.8; 4.8 4.9; 5.1 5.4; 5.4 5.2;
    12 0; 12.3 0.2; -11.8 -0.1; 12.1 0.4];

labels = agglomerative(X, 3, "single", "euclidean");
expected = zeros(16,1);
expected(13:$) = [1 1 2 1]';
assert_checkequal(labels, expected);

labels = agglomerative(X, 3, "complete", "euclidean");
assert_checkequal(labels, expected);

labels = agglomerative(X, 3, "ward", "euclidean");
expected = [0 0 0 0 0 1 1 1 1 1 1 1 2 2 0 2]';
assert_checkequal(labels, expected);

X = [0 0; 0.1 0.2; -0.1 0.1; 0.2 -0.1;
    1 1; 1.1 1.2; 0.9 1.1; 1.2 0.9;
    5 5; 5.1 5.2; 4.9 5.1; 5.2 4.8];

expected = [1 1 1 1 2 2 2 2 0 0 0 0]';

for method = ["single", "complete", "average", "ward"]
    labels = agglomerative(X, 3, method);
    assert_checkequal(labels, expected);
end

X = [0 0; 0.2 0.1; 0.1 -0.2;
    1 0; 2 0; 3 0; 4 0;
    5 5; 5.2 5.1; 4.9 5.2];

expected = [0 0 0 0 0 0 0 1 1 1]';

for method = ["single", "complete", "average"]
    labels = agglomerative(X, 2, method);
    assert_checkequal(labels, expected);
end

X = [0 0; 1 0; 2 0; 3 0; 4 0; 
    8 0; 8 1; 8 2; 8 3; 8 4;
    4 8; 4.2 8.1; 3.9 8.2; 4.1 7.8];

expected = [1 1 1 1 1 0 0 0 0 0 2 2 2 2]';

for method = ["single", "complete", "average", "weighted", "centroid", "median", "ward"];
    labels = agglomerative(X, 3, method);
    assert_checkequal(labels, expected);
end

X = [0 0; 0.1 0.2; -0.2 0.1; 0.2 -0.1; -0.1 -0.2;
    5 5; 5.1 5.2; 4.8 5.1; 5.2 4.9; 4.9 4.8; 
    10 0; 10.2 0.1; 9.8 -0.1; 10.1 -0.2;
    20 20; -5 10];

labels = agglomerative(X, 3, "average");
assert_checkequal(labels, [zeros(14,1); 2; 1]);
labels = agglomerative(X, 5, "average");
assert_checkequal(labels, [0;0;0;0;0;1;1;1;1;1;2;2;2;2;4;3]);

data = [0.0   0.0
0.2   0.1
-0.1   0.2
0.1  -0.2
5.0   5.0
5.2   5.1
4.9   5.2
5.1   4.8
10.0   0.0
10.2   0.1
 9.9  -0.2
10.1   0.2];

Z = linkage(data,"ward");
labels1 = agglomerative(data,3,"ward");
labels2 = agglomerative(Z,3);
assert_checkequal(labels1, labels2);
