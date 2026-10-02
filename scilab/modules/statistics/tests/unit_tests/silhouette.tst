// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for silhouette function
// =============================================================================

X = [0; 1; 10; 11];
labels = [1; 1; 2; 2];
s = silhouette(X, labels);
expected = [19/21; 17/19; 17/19; 19/21];
assert_checkequal(s, expected);
assert_checkequal(size(s), [4 1]);

s2 = silhouette(X, [20; 20; 50; 50]);
assert_checkequal(s, s2);

permutation = [3; 1; 4; 2];
sperm = silhouette(X(permutation), labels(permutation));
assert_checkequal(sperm, s(permutation));

// Cityblock metric
s3 = silhouette(X, labels, "cityblock");
assert_checkequal(s3, expected);

X = [0; 0; 10; 10];
s = silhouette(X, labels);
assert_checkequal(s, ones(4, 1));

X = [0; 0; 0; 0];
s = silhouette(X, labels);
assert_checkequal(s, zeros(4, 1));


// Errors
msg = msprintf(_("%s: Wrong size for input argument #%d: At least 2 samples expected.\n"), "silhouette", 1);
assert_checkerror("silhouette(1, 1)", msg);

msg = msprintf(_("%s: Wrong size for input argument #%d: a vector of length %d expected.\n"), "silhouette", 2, 3);
assert_checkerror("silhouette([1;2;3], [1;1])", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: At least 2 clusters expected.\n"), "silhouette", 2);
assert_checkerror("silhouette([1;2;3], [1;1;1])", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: Number of clusters must be < %d.\n"), "silhouette", 2, 3);
assert_checkerror("silhouette([1;2;3], [1;2;3])", msg);


// Silhouette with kmeans
rand("seed", 0);
data = [rand(30, 2) + 1; rand(30, 2) + 5; rand(30, 2) + 9];
idx = kmeans(data, 3);
scores_km = silhouette(data, idx);

assert_checktrue(mean(scores_km) > 0.5);
assert_checktrue(min(scores_km) >= -1);
assert_checktrue(max(scores_km) <= 1);
assert_checkequal(size(scores_km), [90 1]);

// Silhouette with dbscan
rand("seed", 0);
data = [rand(30, 2); rand(30, 2) + 4];
lab_db = dbscan(data, 0.5, 5);

// Remove noise points (label == -1) before computing silhouette
mask = lab_db <> -1;
data_clean = data(mask, :);
lab_clean = lab_db(mask);

// Need at least 2 clusters to test
n_clust = length(unique(lab_clean));
if n_clust >= 2 then
    scores_db = silhouette(data_clean, lab_clean);
    assert_checktrue(min(scores_db) >= -1);
    assert_checktrue(max(scores_db) <= 1);
    assert_checkequal(size(scores_db, 1), size(data_clean, 1));
end

// Silhouette with meanshift
rand("seed", 0);
data = [rand(30, 2) + 1; rand(30, 2) + 6];
[c, lab_ms] = meanshift(data, 1);

n_clust = length(unique(lab_ms));
if n_clust >= 2 then
    scores_ms = silhouette(data, lab_ms);
    assert_checktrue(mean(scores_ms) > 0.5);
    assert_checktrue(min(scores_ms) >= -1);
    assert_checktrue(max(scores_ms) <= 1);
    assert_checkequal(size(scores_ms, 1), size(data, 1));
end

// Silhouette with agglomerative
data = [0.0  0.0;
        0.2  0.1;
       -0.1  0.2;
        0.1 -0.2;
        5.0  5.0;
        5.2  5.1;
        4.9  5.2;
        5.1  4.8;
       10.0  0.0;
       10.2  0.1;
        9.9 -0.2;
       10.1  0.2];

lab_agg = agglomerative(data, 3, "ward");
scores_agg = silhouette(data, lab_agg);
assert_checktrue(mean(scores_agg) > 0.5);
assert_checktrue(min(scores_agg) >= -1);
assert_checktrue(max(scores_agg) <= 1);
assert_checkequal(size(scores_agg), [12 1]);

// Different linkage methods
for method = ["single", "complete", "average"]
    lab_m = agglomerative(data, 3, method);
    scores_m = silhouette(data, lab_m);
    assert_checktrue(min(scores_m) >= -1);
    assert_checktrue(max(scores_m) <= 1);
end


// Metric tests
rand("seed", 0);
data = [rand(20, 3); rand(20, 3) + 5];
labels = [ones(20, 1); 2 * ones(20, 1)];

// Euclidean (default)
s_euc = silhouette(data, labels);
assert_checktrue(mean(s_euc) > 0.5);

// Cityblock
s_city = silhouette(data, labels, "cityblock");
assert_checktrue(mean(s_city) > 0.5);

// Cosine
s_cos = silhouette(data, labels, "cosine");
assert_checktrue(min(s_cos) >= -1);
assert_checktrue(max(s_cos) <= 1);

// Minkowski with p=3
s_mink = silhouette(data, labels, "minkowski", 3);
assert_checktrue(mean(s_mink) > 0.5);

// Different metrics can yield different scores
assert_checktrue(or(s_euc <> s_city));
