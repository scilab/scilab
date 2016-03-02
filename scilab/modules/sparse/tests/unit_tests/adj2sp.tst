// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

//
// Check the adjacency representation of a simple matrix
xadj = [1 3 4 7 9 11]';
iadj = [3 5 3 1 2 4 3 5 1 4]';
v = [1 2 3 4 5 6 7 8 9 10]';
B=adj2sp(xadj,iadj,v);
A = [
0 0 4 0 9
0 0 5 0 0
1 3 0 7 0
0 0 6 0 10
2 0 0 8 0
];
C=sparse(A);
assert_checkequal ( and(B==C) , %t );
//
// Check the adjacency representation of a simple matrix
xadj = [1 2 3 4 5 5 6 6 7 8 9]';
iadj = [2 5 2 3 1 2 7 6]';
v = [3 7 5 3 6 5 2 3]';
C=adj2sp(xadj,iadj,v);
A = [
0 0 0 0 0 6 0 0 0 0
3 0 5 0 0 0 0 5 0 0
0 0 0 3 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 7 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 3
0 0 0 0 0 0 0 0 2 0
];
B=sparse(A);
assert_checkequal ( and(B==C) , %t );
//
// Test the C=adj2sp(xadj,iadj,v,mn) syntax
xadj = [1 2 3 4 5 5 6 6 7 8 9]';
iadj = [2 5 2 3 1 2 7 6]';
v = [3 7 5 3 6 5 2 3]';
mn=[7 10];
C=adj2sp(xadj,iadj,v,mn);
A = [
0 0 0 0 0 6 0 0 0 0
3 0 5 0 0 0 0 5 0 0
0 0 0 3 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 7 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 3
0 0 0 0 0 0 0 0 2 0
];
B=sparse(A);
assert_checkequal ( and(B==C) , %t );

