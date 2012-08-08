// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 11612 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11612
//
// <-- Short Description -->
// index of the max value of a negative sparse was wrong

a = -1 * rand(3,2);
a(2) = 0;
A = sparse(a);
[m, i] = max(a);
[m1, i1] = max(A);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a');
[m1, i1] = max(A');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:));
[m1, i1] = max(A(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:)');
[m1, i1] = max(A(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

a = [-3 -4;0 -1;-5 -6];
A = sparse(a);
[m, i] = max(a);
[m1, i1] = max(A);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a');
[m1, i1] = max(A');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:));
[m1, i1] = max(A(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(a(:)');
[m1, i1] = max(A(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

b = [-5 0;-1 -6];
B = sparse(b);
[m, i] = max(b);
[m1, i1] = max(B);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b');
[m1, i1] = max(B');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b(:));
[m1, i1] = max(B(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(b(:)');
[m1, i1] = max(B(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

s = rand(3,2);
s(2) = 0;
S = sparse(s);
[m, i] = min(s);
[m1, i1] = min(S);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s');
[m1, i1] = min(S');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s(:));
[m1, i1] = min(S(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = min(s(:)');
[m1, i1] = min(S(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

v = [1 4;7 6];
V = sparse(v);
[m, i] = max(v);
[m1, i1] = max(V);
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v');
[m1, i1] = max(V');
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v(:));
[m1, i1] = max(V(:));
assert_checkequal(m1,m);
assert_checkequal(i1,i);

[m, i] = max(v(:)');
[m1, i1] = max(V(:)');
assert_checkequal(m1,m);
assert_checkequal(i1,i);
