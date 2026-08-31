// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for linkage function
// =============================================================================

x = [1 2 10 3 9 8];
Z = linkage(x, "single");
assert_checkequal(Z, [1 2 1;3 5 2;4 6 8]);

Z = linkage(x, "complete");
assert_checkequal(Z, [1 2 1;3 5 3;4 6 10]);

Z = linkage(x, "average");
assert_checkequal(Z, [1 2 1;3 5 2.5;4 6 9]);

Z = linkage(x, "weighted");
assert_checkequal(Z, [1 2 1;3 5 2.5;4 6 8.75]);

Z = linkage(x, "centroid");
assert_checkalmostequal(Z, [1 2 1;3 5 2.5;4 6 8.95048105]);

Z = linkage(x, "median");
assert_checkalmostequal(Z, [1 2 1;3 5 2.5;4 6 8.69266933]);

Z = linkage(x, "ward");
assert_checkalmostequal(Z, [1 2 1;3 5 2.88675135;4 6 10.96205577]);


X = [2  8  0
    1  9  9
    3  4  2
    8  1  6
    5  5  5
    7  2  1];

Z1 = linkage(X, "single");
Z2 = linkage(pdist(X), "single");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "complete");
Z2 = linkage(pdist(X), "complete");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "average");
Z2 = linkage(pdist(X), "average");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "weighted");
Z2 = linkage(pdist(X), "weighted");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "centroid");
Z2 = linkage(pdist(X), "centroid");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "median");
Z2 = linkage(pdist(X), "median");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "ward");
Z2 = linkage(pdist(X), "ward");
assert_checkequal(Z1, Z2);

metric = "cityblock";
Z1 = linkage(X, "single", metric);
Z2 = linkage(pdist(X, metric), "single");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "complete", metric);
Z2 = linkage(pdist(X, metric), "complete");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "average", metric);
Z2 = linkage(pdist(X, metric), "average");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "weighted", metric);
Z2 = linkage(pdist(X, metric), "weighted");
assert_checkequal(Z1, Z2);

metric = {"minkowski", 2};
Z1 = linkage(X, "single", metric);
Z2 = linkage(pdist(X, metric{1}, metric{2}), "single");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "complete", metric);
Z2 = linkage(pdist(X, metric{1}, metric{2}), "complete");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "average", metric);
Z2 = linkage(pdist(X, metric{1}, metric{2}), "average");
assert_checkequal(Z1, Z2);

Z1 = linkage(X, "weighted", metric);
Z2 = linkage(pdist(X, metric{1}, metric{2}), "weighted");
assert_checkequal(Z1, Z2);

// Errors
msg = msprintf(_("%s: Wrong value for input argument #%d: Euclidean metric with %s method expected.\n"), "linkage", 3, sci2exp(["centroid", "median", "ward"]));
assert_checkerror("linkage(X, ""centroid"", metric)", msg);
assert_checkerror("linkage(X, ""median"", metric)", msg);
assert_checkerror("linkage(X, ""ward"", metric)", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "linkage", 2, sci2exp(["single","complete","average","weighted","centroid","median","ward"]));
assert_checkerror("linkage(X, ""method"")", msg);
