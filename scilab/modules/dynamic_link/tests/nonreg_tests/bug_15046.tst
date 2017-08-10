// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15046 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15046
//
// <-- Short Description -->
// call() couldn't mix inputs and outputs

ilib_verbose(0);
cd TMPDIR;
sub2 = [ ..
"      subroutine sub2(n, m)"; ..
"       implicit none"; ..
"       integer n, m"; ..
"       m=n+1"; ..
"      end"
];
mputl(sub2, "sub2.f");
ilib_for_link("sub2", "sub2.f", [], "f");
exec loader.sce;

N = 3;
[M, K] = call("sub2", N, 1, "i", "out", [1, 1], 1, "i", [1, 1], 2, "i"); // M = N = 3, K = N+1 = 4

assert_checkequal([M K], [3 4]);

M = call("sub2", N, 1, "i", "out", [1, 1], 2, "i"); // M = N+1 = 4

assert_checkequal(M, 4);
