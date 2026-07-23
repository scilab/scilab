// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// =============================================================================
// Tests for pca function
// =============================================================================
msg = msprintf(_("%s: Wrong number of input argument: At least %d expected.\n"), "pca", 1);
assert_checkerror("pca()", msg);
msg = msprintf(_("%s: unknown option ""%s"".\n"), "pca", "centers");
assert_checkerror("pca([1 2 1;2 1 3;3 2 3], ""centers"", %f)", msg);
assert_checktrue(execstr("pca([1 2 1;2 1 3;3 2 3], ""CeNtErEd"", %f)", "errcatch") == 0); // Test case-sensitivity on options

// =============================================================================
a=rand(100,10,'n');
[r1,r2,r3] = pca(a);
// =============================================================================
assert_checkequal(size(r1), [10 10]);
assert_checkequal(size(r2), [100 10]);
assert_checkequal(size(r3), [10 1]);
// =============================================================================

x = [1 2 1; 2 3 1; 0 2 1];
[comprinc, scores, l, tsquare, explained, mu] = pca(x);
assert_checkequal(size(comprinc), [3 2]);
assert_checkequal(size(scores), [3 2]);
assert_checkequal(size(l), [2 1]);
assert_checkequal(size(tsquare), [3 1]);
assert_checkequal(size(explained), [2 1]);
assert_checkequal(size(mu), [1 3]);
assert_checkalmostequal(scores * comprinc(:, 1:2)' + repmat(mu, 3, 1), x, [], 1e-15);
assert_checkalmostequal(sum(explained), 100);
assert_checkalmostequal(l, [1.2675919; 0.0657415], 1d-6);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Centered", %f);
assert_checkequal(size(comprinc), [3 3]);
assert_checkequal(size(scores), [3 3]);
assert_checkequal(size(l), [3 1]);
assert_checkequal(size(tsquare), [3 1]);
assert_checkequal(size(explained), [3 1]);
assert_checkequal(size(mu), [1 3]);
assert_checkalmostequal(scores * comprinc', x, [], 1e-15);
assert_checkalmostequal(sum(explained), 100);

x = [6 6 5 5; 8 8 8 8; 6 7 11 9; 14 14 15 15; 14 14 9 12; 11 10 5 7; 5 7 14 11; 13 12 8 9; 9 9 12 12];
[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Economy", %t);
assert_checkalmostequal(scores * comprinc' + repmat(mu, 9, 1), x);
assert_checkalmostequal(sum(explained), 100);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Economy", %t, "NumComponents", 2);
assert_checkequal(size(comprinc), [4 2]);
assert_checkequal(size(scores), [9 2]);
assert_checkequal(size(l), [2 1]);
assert_checkequal(size(tsquare), [9 1]);
assert_checkequal(size(explained), [2 1]);
assert_checkequal(size(mu), [1 4]);
assert_checkalmostequal(sum(explained), 99.361711);

x = [1 2 1; 2 3 1; 0 2 1; 2 3 9;5 -2 7; -1 2 1];
w = [0.5 0.25 0.25 0.5 0.25 0.25];
[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Weights", w);
assert_checkequal(size(comprinc), [3 3]);
assert_checkequal(size(scores), [6 3]);
assert_checkequal(size(l), [3 1]);
assert_checkequal(size(tsquare), [6 1]);
assert_checkequal(size(explained), [3 1]);
assert_checkequal(size(mu), [1 3]);
assert_checkequal(mu, meanf(x, w' * ones(1, 3), 1));
assert_checkalmostequal(sum(explained), 100);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Weights", w, "Centered", %f);
assert_checkequal(size(comprinc), [3 3]);
assert_checkequal(size(scores), [6 3]);
assert_checkequal(size(l), [3 1]);
assert_checkequal(size(tsquare), [6 1]);
assert_checkequal(size(explained), [3 1]);
assert_checkequal(size(mu), [1 3]);
assert_checkequal(mu, zeros(1, 3));
assert_checkalmostequal(sum(explained), 100);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "VariableWeights", "variance");
assert_checkalmostequal(scores * comprinc' + repmat(mu, 6, 1), x, [], 1e-14);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "VariableWeights", [0.3 0.8 0.5]);
assert_checkalmostequal(scores * comprinc' + repmat(mu, 6, 1), x, [], 1e-15);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Weights", w, "VariableWeights", "variance");
assert_checkalmostequal(scores * comprinc' + repmat(mu, 6, 1), x, [], 1e-15);

[comprinc, scores, l, tsquare, explained, mu] = pca(x, "Weights", w, "VariableWeights", [0.3 0.8 0.5]);
assert_checkalmostequal(scores * comprinc' + repmat(mu, 6, 1), x, [], 1e-15);

X = [-1 -1; -2 -1; -3 -2; 1  1; 2  1; 3  2];

// svd and eig - same results
[c1,s1,l1,t1,e1] = pca(X,"Algorithm","svd");
[c2,s2,l2,t2,e2] = pca(X,"Algorithm","eig");

assert_checkalmostequal(abs(c1),abs(c2),1d-12);
assert_checkalmostequal(abs(s1),abs(s2),1d-12);

assert_checkalmostequal(l1,l2,1d-12);
assert_checkalmostequal(e1,e2,1d-12);
assert_checkalmostequal(t1,t2,1d-12);

// NumComponents
[c,s,l,t,e] = pca(X,"NumComponents",1);

assert_checkequal(size(c),[2 1]);
assert_checkequal(size(s),[6 1]);
assert_checkequal(size(l),[1 1]);
assert_checkequal(size(e),[1 1]);

// NumComponents + eig
[c,s,l,t,e] = pca(X,"Algorithm","eig","NumComponents",1);

assert_checkequal(size(c),[2 1]);
assert_checkequal(size(s),[6 1]);
assert_checkequal(size(l),[1 1]);

[c,s]=pca(X);
X0=X-ones(size(X,1),1)*mean(X,1);
assert_checkalmostequal(X0,s*c');

// svd and eigs with NumComponents
[c1,s1,l1,t1,e1]=pca(X,"Algorithm","svd","NumComponents",1);

[c2,s2,l2,t2,e2]=pca(X,"Algorithm","eig","NumComponents",1);

assert_checkalmostequal(abs(c1),abs(c2),1d-12);
assert_checkalmostequal(abs(s1),abs(s2),1d-12);
assert_checkalmostequal(l1,l2,1d-12);
assert_checkalmostequal(e1,e2,1d-12);
assert_checkalmostequal(t1,t2,1d-12);