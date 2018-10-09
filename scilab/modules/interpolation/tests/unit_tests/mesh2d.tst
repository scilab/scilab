// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//, This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

[x,y] = meshgrid(1:4,1:3);

// Test returned boundary
tri1_1 = [1, 5, 8, 7, 3, 8, 10, 6, 9, 10, 11, 12
4, 2, 5, 5, 2, 6, 11, 3, 6, 8, 9, 9 
2, 4, 7, 4, 5, 5, 8, 5, 8, 7, 8, 11];
[tri1_2, cv_hull_1] = mesh2d(x(:), y(:));
assert_checkequal(cv_hull_1, [1, 4, 7, 10, 11, 12, 9, 6, 3, 2, 1]');
assert_checkequal(tri1_1, tri1_2);

// Test with non-convex boundary
tri2_1 = [1, 5, 3, 8, 10, 6, 9, 10, 11, 12.
4, 2, 2, 6, 11, 3, 6, 8, 9, 9. 
2, 4, 5, 5, 8, 5, 8, 7, 8, 11];
tri2_2 = mesh2d(x(:),y(:), [1, 4, 5, 8, 7, 10, 11, 12, 9, 6, 3, 2, 1]);
assert_checkequal(tri2_1, tri2_2);

// Test with hole
[x,y]=meshgrid(1:4,1:4);
[tri3_1, cv_hull_3] = mesh2d(x(:), y(:));
tri3_2 = mesh2d(x(:), y(:), [cv_hull_3; 6; 10; 11; 7; 6]);
tri3_3 = [1, 6, 10, 9, 3, 13, 7, 8, 15, 13, 4, 14, 8, 15, 12, 16
5, 2, 6, 6, 2, 14, 3, 7,  12, 10, 3, 11, 4, 11, 8,  12
2, 5, 9, 5, 6, 10, 6, 11, 11, 9, 7, 10, 7, 14, 11, 15];
assert_checkequal(tri3_2, tri3_3);
