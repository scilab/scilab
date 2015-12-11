// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 8779 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8779
//
// <-- Short Description -->
//    gsort() did not preserve order of equal elements, in lexicographic sort.
// =============================================================================
[y,k] = gsort(ones(8,1), "lr");
assert_checkequal(y, ones(8, 1));
assert_checkequal(k, (1:8)');

[y,k] = gsort(ones(8,1), "lr", "i");
assert_checkequal(y, ones(8, 1));
assert_checkequal(k, (1:8)');

[y,k] = gsort(ones(1,8), "lc");
assert_checkequal(y, ones(1, 8));
assert_checkequal(k, (1:8));

[y,k] = gsort(ones(1,8), "lc", "i");
assert_checkequal(y, ones(1, 8));
assert_checkequal(k, (1:8));

A = [1 2 3;4 5 6;4 5 6;7 8 9;8 9 10;4 5 6];
[x,k] = gsort(A, "lr", "i");
assert_checkequal(k, [1;2;3;6;4;5]);
assert_checkequal(x, [1 2 3;4 5 6;4 5 6; 4 5 6;7 8 9;8 9 10]);
