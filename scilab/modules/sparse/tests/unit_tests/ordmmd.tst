// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->



A = [
4. 1. 2. 0.5 2.
1. 0.5 0. 0. 0.
2. 0. 3. 0. 0.
0.5 0. 0. 5/8 0.
2. 0. 0. 0. 16.
];
A = sparse(A);
[xadj,iadj,val]=sp2adj(A);
n = size(A,"r");
[perm,invp,nofsub]=ordmmd(xadj,iadj,n);
assert_checkequal(perm,[5;4;3;2;1]);
assert_checkequal(invp,[5;4;3;2;1]);
assert_checkequal(nofsub,8);
